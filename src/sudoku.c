/*
vim: noexpandtab:ts=4:sts=4:sw=4

nudoku

Copyright (C) 2014 - 2024 Michael "jubalh" Vetter - jubalh _a-t_ iodoru.org

LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* INCLUDES */
#include <stdlib.h>		/* rand, malloc */
#include <time.h>		/* time */
#include <string.h>		/* strdup */
#include "sudoku.h"		/* enum */
#include "gettext.h"	/* gettext */
#include "unistd.h"		/* sleep */

#define _(x) gettext(x)

/* FUNCTIONS */

/* SOLVER */
bool is_valid_puzzle(char puzzle[STREAM_LENGTH])
{
	int rowEntry, colEntry, squareEntry;

	int rowEntryCounter[9] = {0};
	int colEntryCounter[9] = {0};
	int squareEntryCounter[9] = {0};

	for(int row=0; row<9; row++)
	{
		// count occurrences of digits in each row, column and square
		for (int col=0; col<9; col++)
		{
			// get the entries for one row, col and the whole square
			rowEntry = puzzle[row * 9 + col];
			colEntry = puzzle[col * 9 + row];
			squareEntry = puzzle[(((row % 3) + (row / 3) * 9) * 3 ) + (col % 3 + (col / 3) * 9)];
			// if its not an empty field
			// count occurence
			// -48 (ASCII to number) - 1 (array index starts at 0)
			if (rowEntry != '.')
			{
				rowEntryCounter[rowEntry - 49]++;
			}
			if (colEntry!= '.')
			{
				colEntryCounter[colEntry - 49]++;
			}
			if (squareEntry != '.')
			{
				squareEntryCounter[squareEntry - 49]++;
			}
		}
		// check if any digit occurs more than once per row, column or square
		for (int i = 0; i < 9; i++)
		{
			if (rowEntryCounter[i] > 1 || colEntryCounter[i] > 1 || squareEntryCounter[i] > 1)
				return false;
			rowEntryCounter[i] = 0;
			colEntryCounter[i] = 0;
			squareEntryCounter[i] = 0;
		}
	}
	return true;
}

static int get_candidates(char puzzle[STREAM_LENGTH], int pos) {
    int row = pos / 9, col = pos % 9;
    int box_row = (row / 3) * 3, box_col = (col / 3) * 3;
    int mask = 0;

    for (int i = 0; i < 9; i++) {
        if (puzzle[row * 9 + i] != '.') mask |= 1 << (puzzle[row * 9 + i] - '1');
        if (puzzle[i * 9 + col] != '.') mask |= 1 << (puzzle[i * 9 + col] - '1');
        if (puzzle[(box_row + i / 3) * 9 + box_col + i % 3] != '.')
            mask |= 1 << (puzzle[(box_row + i / 3) * 9 + box_col + i % 3] - '1');
    }
    return 0x1FF & ~mask;
}

static int find_best_cell(char puzzle[STREAM_LENGTH], int *cand_out) {
    int best_pos = -1, best_count = 10;

    for (int pos = 0; pos < 81; pos++) {
        if (puzzle[pos] != '.') continue;

        int candidates = get_candidates(puzzle, pos);
        int n = __builtin_popcount(candidates); // single CPU instruction

        if (n == 0) { *cand_out = 0; return -2; } // dead end
        if (n < best_count) {
            best_count = n;
            best_pos = pos;
            *cand_out = candidates;
            if (n == 1) break;
        }
    }
    return best_pos; // -1 if solved
}

// counts solutions (always backtracks)
static int count_solutions(char puzzle[STREAM_LENGTH], int count) {
    int candidates;
    int pos = find_best_cell(puzzle, &candidates);

    if (pos == -2) return count;     // dead end
    if (pos == -1) return count + 1; // solved

    for (int num = 0; num < 9 && count < 2; num++) {
        if (candidates & (1 << num)) {
            puzzle[pos] = '1' + num;
            count = count_solutions(puzzle, count);
            puzzle[pos] = '.'; // always reset
        }
    }
    return count;
}

// Fills puzzle with first solution (keeps values)
static bool fill_puzzle(char puzzle[STREAM_LENGTH]) {
    int candidates;
    int pos = find_best_cell(puzzle, &candidates);

    if (pos == -2) return false; // dead end
    if (pos == -1) return true;  // solved

    for (int num = 0; num < 9; num++) {
        if (candidates & (1 << num)) {
            puzzle[pos] = '1' + num;
            if (fill_puzzle(puzzle)) return true; // keep solution
            puzzle[pos] = '.';
        }
    }
    return false;
}

int solve(char puzzle[STREAM_LENGTH]) {
    if (!is_valid_puzzle(puzzle))
        return 0;

    // count solutions on a copy
    char copy[STREAM_LENGTH];
    strncpy(copy, puzzle, STREAM_LENGTH);
    int count = count_solutions(copy, 0);

    // fill original if solvable
    if (count >= 1)
        fill_puzzle(puzzle);

    return count;
}
/* GENERATOR */
/* Generator code is influenced by: http://rubyquiz.strd6.com/quizzes/182-sudoku-generator */
static int rand_int(int n)
{
	int rnd;
	int limit = RAND_MAX - RAND_MAX % n;

	do {
		rnd = rand();
	} while (rnd >= limit);
	return (rnd % n);
}

static void shuffle(char *array, int n)
{
	int i, j;
	char tmp;

	for (i = n - 1; i > 0; i--)
	{
		j = rand_int(i + 1);
		tmp = array[j];
		array[j] = array[i];
		array[i] = tmp;
	}
}

static char* create_random_numbers()
{
	char numbers[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
	shuffle(numbers, 9);
	return strdup(numbers);
}

static char* generate_seed()
{
	char *stream = (char*)malloc(STREAM_LENGTH);
	int index = 0;
	int iSquare = 0;

	char* upperleft = create_random_numbers();
	char* center = create_random_numbers();
	char* lowerright = create_random_numbers();

	//first three rows
	for (int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
			stream[index++] = upperleft[iSquare++];
		for(int j = 0; j < 6 ; j++)
			stream[index++] = '.';
	}
	iSquare = 0;
	//second three rows
	for (int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3 ; j++)
			stream[index++] = '.';
		for(int j = 0; j < 3; j++)
			stream[index++] = center[iSquare++];
		for(int j = 0; j < 3 ; j++)
			stream[index++] = '.';
	}
	iSquare = 0;
	//third three rows
	for (int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 6 ; j++)
			stream[index++] = '.';
		for(int j = 0; j < 3; j++)
			stream[index++] = lowerright[iSquare++];
	}

	stream[81] = '\0';

	free(upperleft);
	free(center);
	free(lowerright);

	return stream;
}

static void punch_holes(char *stream, int count)
{
	int i = 0;
	while (i < count)
	{
		int random = rand() % 80 + 1;
		char temp = stream[random];

		if (stream[random] != '.')
		{
			stream[random] = '.';

			char puzzle_copy[STREAM_LENGTH];
			strncpy(puzzle_copy, stream, STREAM_LENGTH);

			// check if puzzle has only 1 solution
			if (solve(puzzle_copy) == 1)
			{
				i++;
			}
			else
			{
				// restore removed value
				stream[random] = temp;
			}
		}
	}
}

const char* difficulty_to_str(DIFFICULTY level)
{
	switch(level)
	{
		case D_HARD:
			return _("hard");
		case D_NORMAL:
			return _("normal");
		case D_EASY:
		default:
			return _("easy");
	}
}

char* generate_puzzle(int holes)
{
	char* stream;

	stream = generate_seed();
	solve(stream);
	punch_holes(stream, holes);
	return stream;
}

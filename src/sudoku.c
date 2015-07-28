/*
nudoku

Copyright (C) 2014 Michael "jubalh" Vetter - jubalh _a-t_ openmailbox.org

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
#include <stdbool.h>	/* bool */
#include <stdlib.h>		/* rand, malloc */
#include <time.h>		/* time */
#include <string.h>		/* strdup */
#include "sudoku.h"		/* enum */

/* FUNCTIONS */
void board_to_stream(int board[9][9], char *stream)
{
	int i, j, n=0;
	for (i = 0; i<9; i++)
	{
		for (j = 0; j<9; j++)
			stream[n++] = board[i][j]+48;
	}
}

/*TODO: so far just ignoring longer stream. maybe check for 81*/
bool init_board(int board[9][9], char *stream)
{
	int row, col;

	for(row=0; row < 9; row++)
	{
		for(col=0; col < 9; col++)
		{
			char *p = stream++;
			// In the stream there should only be numbers or
			// empty field (.)
			if(!((*p >= 49 && *p <= 57) || *p == '.' ))
				return false;
			if (*p == '.')
				board[row][col] = 0;
			else
				board[row][col] = *p - 48;
		}
	}
	return true;
}

void copy_board(int dst[9][9], int src[9][9])
{
	int row, col;

	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col ++)
		{
			dst[row][col] = src[row][col];
		}
	}
}

bool board_is_equal(int one[9][9], int two[9][9])
{
	int row, col;

	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col ++)
		{
			if (one[row][col] != two[row][col])
				return false;
		}
	}
	return true;
}

/* SOLVER */
/* Solver code has been taken from sb0rg: https://codereview.stackexchange.com/questions/37430/sudoku-solver-in-c */
bool isAvailable(int puzzle[9][9], int row, int col, int num)
{
	int i;
	int rowStart = (row/3) * 3;
	int colStart = (col/3) * 3;

	for(i=0; i<9; ++i)
	{
		if (puzzle[row][i] == num) return false;
		if (puzzle[i][col] == num) return false;
		if (puzzle[rowStart + (i%3)][colStart + (i/3)] == num) return false;
	}
	return true;
}

int solve(int puzzle[9][9], int row, int col)
{
	int i;
	if(row<9 && col<9)
	{
		if(puzzle[row][col] != 0)
		{
			if((col+1)<9)
				return solve(puzzle, row, col+1);
			else if((row+1)<9)
				return solve(puzzle, row+1, 0);
			else
				return 1;
		}
		else
		{
			for(i=0; i<9; ++i)
			{
				if(isAvailable(puzzle, row, col, i+1))
				{
					puzzle[row][col] = i+1;

					if(solve(puzzle, row, col))
						return 1;
					else
						puzzle[row][col] = 0;
				}
			}
		}
		return 0;
	}
	else
		return 1;
}

/* GENERATOR */
/* Generator code is influenced by: http://rubyquiz.strd6.com/quizzes/182-sudoku-generator */
static int rand_int(int n) {
	int rnd;
	int limit = RAND_MAX - RAND_MAX % n;

	do {
		rnd = rand();
	} while (rnd >= limit);
	return (rnd % n);
}

void shuffle(char *array, int n) {
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

char* create_random_numbers()
{
	char numbers[10] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
	shuffle(numbers, 9);
	return strdup(numbers);
}

char* generate_seed()
{
	char *stream = (char*)malloc(82);

	srand(time(NULL));

	char* upperleft = create_random_numbers();
	char* center = create_random_numbers();
	char* lowerright = create_random_numbers();

	int index=0;
	int iSquare = 0;

	//first three rows
	for (int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
			stream[index++] = upperleft[iSquare++];
		for(int j=0; j < 6 ; j++)
			stream[index++] = '.';
	}
	iSquare = 0;
	//second three rows
	for (int i=0; i<3; i++)
	{
		for(int j=0; j < 3 ; j++)
			stream[index++] = '.';
		for(int j=0; j<3; j++)
			stream[index++] = center[iSquare++];
		for(int j=0; j < 3 ; j++)
			stream[index++] = '.';
	}
	iSquare = 0;
	//third three rows
	for (int i=0; i<3; i++)
	{
		for(int j=0; j < 6 ; j++)
			stream[index++] = '.';
		for(int j=0; j<3; j++)
			stream[index++] = upperleft[iSquare++];
	}

	free(upperleft);
	free(center);
	free(lowerright);

	return stream;
}

void punch_holes(char *stream, int count)
{
	int i=0;
	while(i<count)
	{
		int random = rand() % 80 + 1;
		if (stream[random] != '.')
		{
			stream[random] = '.';
			i++;
		}
	}
}

char* difficulty_to_str(DIFFICULTY level)
{
	switch(level)
	{
		case D_HARD:
			return "hard";
		case D_NORMAL:
			return "normal";
		case D_EASY:
		default:
			return "easy";
	}
}

int get_holes(DIFFICULTY level)
{
	int holes;
	switch (level)
	{
		case D_HARD:
			/*holes = rand() % 60 + 30;*/
			holes = 50;
			break;
		case D_NORMAL:
			holes = 40;
			break;
		case D_EASY:
		default:
			holes = 30;
			break;
	}
	return holes;
}

char* generate_puzzle(int holes)
{
	char* stream;
	int board[9][9];

	stream = generate_seed();
	init_board(board, stream);
	solve(board, 0, 0);
	board_to_stream(board, stream);
	punch_holes(stream, holes);
	return stream;
}


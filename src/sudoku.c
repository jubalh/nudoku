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

/*#include <stdio.h>*/
/*#include <unistd.h>*/

/* FUNCTIONS */

/* SOLVER */
/* Solver code has been taken from sb0rg: https://codereview.stackexchange.com/questions/37430/sudoku-solver-in-c */
bool isAvailable(char puzzle[82], int row, int col, int num)
{
	int i;
	int rowStart = (row/3) * 3;
	int colStart = (col/3) * 3;

	for(i=0; i<9; ++i)
	{
		if (puzzle[row * 9 + i] - 48 == num) return false;
		if (puzzle[i * 9 + col] - 48 == num) return false;
		if (puzzle[(rowStart + (i % 3)) * 9 + (colStart + (i / 3))] - 48 == num) return false;
	}
	return true;
}

int solve(char puzzle[82], int row, int col)
{
	int i;
	if(row<9 && col<9)
	{
		if(puzzle[row * 9 + col] != '.')
		{
			if((col + 1) < 9)
				return solve(puzzle, row, col + 1);
			else if((row + 1) < 9)
				return solve(puzzle, row + 1, 0);
			else
				return 1;
		}
		else
		{
			for(i=0; i<9; ++i)
			{
				if(isAvailable(puzzle, row, col, i+1))
				{
					puzzle[row * 9 + col] = i + 1 + 48;

					if(solve(puzzle, row, col))
						return 1;
					else
						puzzle[row * 9 + col] = '.';
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

	stream[81] = '\0';

	free(upperleft);
	free(center);
	free(lowerright);

	return stream;
}

void punch_holes(char *stream, int count)
{
	int i=0;
	int rounds=0;
	/*int c;*/
#define MAX_PUNCH_TRIES 50000
	while(i<count && rounds < MAX_PUNCH_TRIES )
	{
		int random = rand() % 80 + 1;
		if (stream[random] != '.')
		{
			int solutions = 1;
			if (i>2)
			{
				solutions = 0;
				//put in all numbers so we can test how many solutions
				for (int j=49; j<58; j++)
				{
					char tmp_board[82];

					strcpy(tmp_board, stream);

					/*printf("chage %d to %d at position %d\n", tmp_board[random], j, random);*/
					tmp_board[random] = j;

					if (solve(tmp_board, 0, 0) == 0)
					{
						solutions++;
					}
				}
				/*if (solutions != 0)*/
					/*printf("solutions %d\n", solutions);*/
			}
			if (solutions == 1)
			{
				//when unique solution then we can use it
				stream[random] = '.';
				i++;
			}
		}
		rounds++;
	}
	/*printf("stream %s\n", stream);*/
	/*c = getchar();*/
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

	stream = generate_seed();
	solve(stream, 0, 0);
	punch_holes(stream, holes);
	return stream;
}


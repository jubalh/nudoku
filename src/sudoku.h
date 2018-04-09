// vim: noexpandtab:ts=4:sts=4:sw=4

#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>	/* bool */

#define STREAM_LENGTH 82

typedef enum { D_EASY, D_NORMAL, D_HARD } DIFFICULTY;

int		get_holes(DIFFICULTY level);
char*	difficulty_to_str(DIFFICULTY level);
char*	generate_puzzle(int holes);
int		solve(char puzzle[STREAM_LENGTH]);
bool	is_valid_puzzle(char puzzle[STREAM_LENGTH]);

#endif // SUDOKU_H

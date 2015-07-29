#ifndef SUDOKU_H
#define SUDOKU_H

typedef enum { D_EASY, D_NORMAL, D_HARD } DIFFICULTY;

int		get_holes(DIFFICULTY level);
char*	difficulty_to_str(DIFFICULTY level);
bool	board_is_equal(int one[9][9], int two[9][9]);
char*	generate_puzzle(int holes);
int		solve(char puzzle[82], int row, int col);

#define solve_sudoku(board) solve(board, 0, 0);

#define STREAM_LENGTH 81

#endif // SUDOKU_H

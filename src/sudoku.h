#ifndef SUDOKU_H
#define SUDOKU_H

typedef enum { D_EASY, D_NORMAL, D_HARD } DIFFICULTY;

int get_holes(DIFFICULTY level);
char* difficulty_to_str(DIFFICULTY level);
bool init_board(int board[9][9], char *stream);
//void copy_board(int dst[9][9], int src[9][9]);
bool board_is_equal(int one[9][9], int two[9][9]);
char* generate_puzzle(int holes);
int solve(char *puzzle, int row, int col);

#define solve_sudoku(board) solve(board, 0, 0);

#endif // SUDOKU_H

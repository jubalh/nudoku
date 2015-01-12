#ifndef SUDOKU_H
#define SUDOKU_H

bool init_board(int board[9][9], char *stream);
void copy_board(int dst[9][9], int src[9][9]);
bool compare_boards(int one[9][9], int two[9][9]);
char* generate_puzzle(void);
int solve(int puzzle[9][9], int row, int col);
#define solve_sudoku(board) solve(board, 0, 0);

#endif // SUDOKU_H

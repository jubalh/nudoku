#ifndef SUDOKU_H
#define SUDOKU_H

int solve(int puzzle[][9], int row, int col);
#define solve_sudoku(board) solve(board, 0, 0);

#endif // SUDOKU_H

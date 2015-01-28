#ifndef SUDOKU_H
#define SUDOKU_H

typedef enum { D_EASY, D_NORMAL, D_HARD } DIFFICULTY;

int get_holes(DIFFICULTY level);
char* difficulty_to_str(DIFFICULTY level);
bool init_board(int board[9][9], char *stream);
void copy_board(int dst[9][9], int src[9][9]);
bool compare_boards(int one[9][9], int two[9][9]);
char* generate_puzzle(int holes);
int solve(int puzzle[9][9], int row, int col);

#define solve_sudoku(board) solve(board, 0, 0);

#endif // SUDOKU_H

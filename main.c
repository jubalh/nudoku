/*
nudoku 0.1

Copyright (C) 2014 Michael "jubalh" Vetter - g.bluehut _a-t_ gmail.com

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

/*
 * Conventions:
 * board is the model of the sudoku. a two dimensional array.
 * grid is the visual represantation of the sudoku.
 */

/* INCLUDES */
#include <stdlib.h>		/* rand, srand */
#include <unistd.h>		/* getopt */
#include <ncurses.h>	/* ncurses */
#include <time.h>		/* time */
#include "sudoku.h"

/* DEFINES */
#define VERSION				"0.1"
#define GRID_LINES			19
#define GRID_COLS			37
#define GRID_Y				3
#define GRID_X				3
#define INFO_LINES			19
#define INFO_COLS			20
#define INFO_Y				3
#define INFO_X				GRID_X + GRID_COLS + 5
#define GRID_NUMBER_START_Y 1
#define GRID_NUMBER_START_X 2
#define GRID_LINE_DELTA		4
#define GRID_COL_DELTA		2
#define STATUS_LINES		1
#define STATUS_COLS			GRID_COLS + INFO_COLS
#define STATUS_Y			1
#define STATUS_X			GRID_X

#ifdef DEBUG
#define EXAMPLE_STREAM "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......"
#endif // DEBUG

/* GLOBALS */
bool g_useColor = true;
bool g_playing = false;
WINDOW *grid, *infobox, *status;
int plain_board[9][9];
int user_board[9][9];

/* FUNCTIONS */
void print_version(void)
{
	printf("nudoku version " VERSION "\n\n\
Copyright (C) Michael Vetter 2014\n\
License GPLv3+: GNU GPL version 3 or later.\n\
This is free software, you are free to modify and redistribute it.\n");
#ifdef DEBUG
	printf("Debug enabled\n");
#endif // DEBUG
}

void print_usage(void)
{
	printf("nudoku [option]\n\n");
	printf("Options:\n");
	printf("-h help:\t\t\tPrint this help\n");
	printf("-v version:\t\tPrint version\n");
	printf("-c nocolor:\t\tDo not use colors\n");
	printf("-l load filename:\tLoad sudoku from file\n");
}

void parse_arguments(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, "hvcl:")) != -1)
	{
		switch (opt)
		{
			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'v':
				print_version();
				exit(EXIT_SUCCESS);
			case 'c':
				g_useColor = false;
				break;
			case 'l':
				printf("not yet implemented\n");
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

void cleanup(void)
{
	endwin();
}

void init_curses(void)
{
	initscr();
	clear();
	atexit(cleanup);
	cbreak();
	noecho();

	if(g_useColor)
	{
		if(has_colors())
		{
			start_color();
			init_pair(1, COLOR_GREEN, COLOR_BLACK);
			init_pair(2, COLOR_BLUE, COLOR_BLACK);
			// user input color
			init_pair(3, COLOR_CYAN, COLOR_BLACK);
		}
		else
		{
			printw( "Your terminal doesn't support colors.\nTry the nocolor (-c) option.\n");
			getch();
			exit(EXIT_FAILURE);
		}
	}
}

void _draw_grid()
{
	int i, j;

	for(i = 0;i < 10;i++)
	{
		for(j = 0;j < 10;j++)
		{
			if (g_useColor)
			{
				if(i % 3 == 0)
					wattron(grid, A_BOLD|COLOR_PAIR(2));
				if(j % 3 == 0)
					wattron(grid, A_BOLD|COLOR_PAIR(2));
			}
			wprintw(grid, "+");
			if(g_useColor && (j % 3 == 0) && (i % 3 != 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
			if(j < 9)
				wprintw(grid, "---");
			if(g_useColor && (i % 3 == 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
		}
		for(j = 0;j < 10 && i < 9;j++)
		{
			if(g_useColor && (j % 3 == 0))
				wattron(grid, A_BOLD|COLOR_PAIR(2));
			if(j > 0)
				wprintw(grid, "   |");
			else
				wprintw(grid, "|");
			if(g_useColor && (j % 3 == 0))
			{
				wattron(grid, A_BOLD|COLOR_PAIR(1));
			}
		}
	}
}

void init_windows(void)
{
	keypad(stdscr, true);

	status = newwin(STATUS_LINES, STATUS_COLS, STATUS_Y, STATUS_X);

	grid = newwin(GRID_LINES, GRID_COLS, GRID_Y, GRID_X);
	_draw_grid();

	infobox = newwin(INFO_LINES, INFO_COLS, INFO_Y, INFO_X);
	if (g_useColor)
	{
		wbkgd(infobox, COLOR_PAIR(2));
	}

	wprintw(infobox, "Commands\n");
	wprintw(infobox, " q - Quit\n");
	wprintw(infobox, " r - Redraw\n");
	wprintw(infobox, " h - Move left\n");
	wprintw(infobox, " l - Move right\n");
	wprintw(infobox, " j - Move down\n");
	wprintw(infobox, " k - Move up\n");
	wprintw(infobox, " c - Check solution\n");
	wprintw(infobox, " N - New puzzle\n");
	wprintw(infobox, " S - Solve puzzle\n");
}

void fill_grid(int board[][9])
{
	int row, col, x, y;
	int n;
	int c;

	wstandend(grid);
	y = GRID_NUMBER_START_Y;
	for(row=0; row < 9; row++)
	{
		x = GRID_NUMBER_START_X;
		for(col=0; col < 9; col++)
		{
			n = board[row][col];
			if(n == 0)
				c = ' ';
			else 
				c = n + 48;
			mvwprintw(grid, y, x, "%c", c);
			x += GRID_LINE_DELTA;
		}
		y += GRID_COL_DELTA;
	}
}

void new_puzzle(void)
{
	char* stream = generate_puzzle();

	init_board(plain_board, stream);
	init_board(user_board, stream);
	free(stream);
	fill_grid(plain_board);

	g_playing = true;
}

bool compare(void)
{
	int tmp_board[9][9];

	copy_board(tmp_board, plain_board);
	solve_sudoku(tmp_board);

	return compare_boards(tmp_board, user_board);
}

int main(int argc, char *argv[])
{
	bool run = true;
	int key, x, y;

	parse_arguments(argc, argv);
	init_curses();
	init_windows();
	srand(time(NULL));

#ifdef DEBUG
	init_board(plain_board, EXAMPLE_STREAM);
	init_board(user_board, EXAMPLE_STREAM);
	fill_grid(plain_board);
	g_playing = true;
#else
	new_puzzle();
#endif // DEBUG

	refresh();
	wrefresh(grid);
	wrefresh(infobox);

	y = GRID_NUMBER_START_Y;
	x = GRID_NUMBER_START_X;
	wmove(grid, y, x);
	while(run)
	{
#ifdef DEBUG
	mvprintw(0, 0, "y: %.2d x: %.2d", y, x);
#endif // DEBUG
		refresh();
		wrefresh(grid);
		key = getch();
		switch(key)
		{
			case 'h':
			case KEY_LEFT:
				if(x>5)
					x -= GRID_LINE_DELTA;
				break;
			case 'l':
			case KEY_RIGHT:
				if(x<34)
					x += GRID_LINE_DELTA;
				break;
			case 'k':
			case KEY_UP:
				if(y>2)
					y -= GRID_COL_DELTA;
				break;
			case 'j':
			case KEY_DOWN:
				if(y<17)
					y += GRID_COL_DELTA;
				break;
			case 'q':
			case 27:
				run = false;
				break;
			case 'r':
				redrawwin(grid);
				redrawwin(infobox);
				break;
			case 'S':
				werase(status);
				mvwprintw(status, 0, 0, "Solving puzzle...");
				refresh();
				wrefresh(status);
				solve_sudoku(plain_board);
				fill_grid(plain_board);
				werase(status);
				mvwprintw(status, 0, 0, "Solved!");
				g_playing = false;
				break;
			case 'N':
				werase(status);
				mvwprintw(status, 0, 0, "Generating puzzle...");
				refresh();
				wrefresh(status);
				new_puzzle();
				werase(status);
				g_playing = true;
				break;
			case 'c':
				if(g_playing)
				{
					werase(status);
					if(compare())
					{
						mvwprintw(status, 0, 0, "Correct!");
						g_playing = false;
					}
					else
					{
						mvwprintw(status, 0, 0, "Not correct");
					}
				}
				break;
			default:
				break;
		}
		/*if user inputs a number*/
		if(key >= 49 && key <= 57 && g_playing)
		{
			int posy = (y-GRID_NUMBER_START_Y)/GRID_COL_DELTA;
			int posx = (x-GRID_NUMBER_START_X)/GRID_LINE_DELTA;
			// if on empty position
			if(user_board[posy][posx] == 0)
			{
				// add inputted number to grid
				wattron(grid, COLOR_PAIR(3));
				wprintw(grid, "%c", key);
				wattroff(grid, COLOR_PAIR(3));
				user_board[posy][posx] = key - 48;
			}
		}
		wmove(grid, y,x);
		refresh();
		wrefresh(status);
		wrefresh(grid);
		wrefresh(infobox);
	}

	endwin();
	return EXIT_SUCCESS;
}


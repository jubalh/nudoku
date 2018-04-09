/*
vim: noexpandtab:ts=4:sts=4:sw=4

nudoku

Copyright (C) 2014 - 2018 Michael "jubalh" Vetter - jubalh _a-t_ iodoru.org

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
#include <stdlib.h>				/* rand, srand */
#include <unistd.h>				/* getopt */
#include <ncurses.h>			/* ncurses */
#include <time.h>				/* time */
#include <string.h>				/* strcmp, strlen */
#include "sudoku.h"				/* sudoku functions */

/* DEFINES */
//#define VERSION				"0.1" //gets set via autotools
#define GRID_LINES				19
#define GRID_COLS				37
#define GRID_Y					3
#define GRID_X					3
#define INFO_LINES				19
#define INFO_COLS				20
#define INFO_Y					3
#define INFO_X					GRID_X + GRID_COLS + 5
#define GRID_NUMBER_START_Y		1
#define GRID_NUMBER_START_X		2
#define GRID_LINE_DELTA			4
#define GRID_COL_DELTA			2
#define STATUS_LINES			1
#define STATUS_COLS				GRID_COLS + INFO_COLS
#define STATUS_Y				1
#define STATUS_X				GRID_X
#define MAX_HINT_RANDOM_TRY		20
#define SUDOKU_LENGTH			STREAM_LENGTH - 1
#define COLOR_HIGHLIGHT			4
#define COLOR_HIGHLIGHT_CURSOR	5

#ifdef DEBUG
#define EXAMPLE_STREAM "4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......"
#endif // DEBUG

/* GLOBALS */
static bool g_useColor = true;
static bool g_playing = false;
static char* g_provided_stream;				/* in case of -s flag the user provides the sudoku stream */
static char plain_board[STREAM_LENGTH];
static char user_board[STREAM_LENGTH];
static DIFFICULTY g_level = D_EASY;
static WINDOW *grid, *infobox, *status;
static bool g_useHighlights = false;

/* FUNCTIONS */
static void print_version(void)
{
	printf("nudoku version " VERSION "\n\n\
Copyright (C) Michael Vetter 2014\n\
License GPLv3+: GNU GPL version 3 or later.\n\
This is free software, you are free to modify and redistribute it.\n");
#ifdef DEBUG
	printf("Debug enabled\n");
#endif // DEBUG
}

static void print_usage(void)
{
	printf("nudoku [option]\n\n");
	printf("Options:\n");
	printf("-h help:\t\tPrint this help\n");
	printf("-v version:\t\tPrint version\n");
	printf("-c nocolor:\t\tDo not use colors\n");
	printf("-d difficulty:\t\tChoose between: easy, normal, hard\n");
	printf("-s stream:\t\tUser provided sudoku stream\n");
}

static bool is_valid_stream(char *s)
{
	char *p = s;
	short n = 0;
	while ((*p) != '\0')
	{
		if (n++ > SUDOKU_LENGTH)
			break;

		if(!((*p >= 49 && *p <= 57) || *p == '.' ))
		{
			printf("Character %c at position %d is not allowed.\n", *p, n);
			return false;
		}
		p++;
	}

	if (n != SUDOKU_LENGTH )
	{
		printf("Stream has to be %d characters long.\n", SUDOKU_LENGTH);
		return false;
	}

	if (!is_valid_puzzle(s))
	{
		printf("Stream does not represent a valid sudoku puzzle.\n");
		return false;
	}

	return true;
}

static void parse_arguments(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, "hvcs:d:")) != -1)
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
			case 's':
				if (!is_valid_stream(optarg))
					exit(EXIT_FAILURE);
				g_provided_stream = strdup(optarg);
				break;
			case 'd':
				if (strcmp(optarg, "easy") == 0)
					g_level = D_EASY;
				else if (strcmp(optarg, "normal") == 0)
					g_level = D_NORMAL;
				else if (strcmp(optarg, "hard") == 0)
					g_level = D_HARD;
				else
				{
					print_usage();
					exit(EXIT_FAILURE);
				}
				break;
			default:
				print_usage();
				exit(EXIT_FAILURE);
		}
	}
}

static void cleanup(void)
{
	endwin();
}

static void init_curses(void)
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
			// Highlight color
			init_pair(COLOR_HIGHLIGHT, COLOR_BLACK, COLOR_WHITE);
			// Cursor highlight color
			init_pair(COLOR_HIGHLIGHT_CURSOR, COLOR_BLACK, COLOR_MAGENTA);
		}
		else
		{
			printw( "Your terminal doesn't support colors.\nTry the nocolor (-c) option.\n");
			getch();
			exit(EXIT_FAILURE);
		}
	}
}

static void _draw_grid()
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

static void init_windows(void)
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

	if (g_useColor)
		wattron(infobox, A_BOLD|COLOR_PAIR(2));

	wprintw(infobox, "nudoku %s\n", VERSION);

	if (!g_provided_stream)
		wprintw(infobox, "level: %s\n\n", difficulty_to_str(g_level) );
	else
		wprintw(infobox, "\n\n");

	if (g_useColor)
	{
		wattroff(infobox, A_BOLD|COLOR_PAIR(2));
		wattron(infobox, COLOR_PAIR(1));
	}
	wprintw(infobox, "Movement\n");
	wprintw(infobox, " h - Move left\n");
	wprintw(infobox, " j - Move down\n");
	wprintw(infobox, " k - Move up\n");
	wprintw(infobox, " l - Move right\n\n");
	wprintw(infobox, "Commands\n");
	wprintw(infobox, " c - Check solution\n");
	wprintw(infobox, " H - Give a hint\n");
	wprintw(infobox, " M - Toggle marks\n");
	wprintw(infobox, " N - New puzzle\n");
	wprintw(infobox, " Q - Quit\n");
	wprintw(infobox, " r - Redraw\n");
	wprintw(infobox, " S - Solve puzzle\n");
	wprintw(infobox, " x - Delete number\n");
	if (g_useColor)
		wattroff(infobox, COLOR_PAIR(1));
}

static int get_character_at_grid(char* board, int x, int y)
{
	int posx, posy;
	posy = (y-GRID_NUMBER_START_Y)/GRID_COL_DELTA;
	posx = (x-GRID_NUMBER_START_X)/GRID_LINE_DELTA;
	return board[posy*9+posx];
}

static void fill_grid(char *board, int x_cursor, int y_cursor)
{
	int row, col, x, y;
	int n;
	int c;
	int selected;

	selected = get_character_at_grid(board, x_cursor, y_cursor);
	wstandend(grid);
	y = GRID_NUMBER_START_Y;
	for(row=0; row < 9; row++)
	{
		x = GRID_NUMBER_START_X;
		for(col=0; col < 9; col++)
		{
			n = board[row*9+col];
			if(n == '.')
				c = ' ';
			else
				c = n;
			if (g_useHighlights && selected == c)
			{
				if (x == x_cursor && y == y_cursor)
					wattron(grid, COLOR_PAIR(COLOR_HIGHLIGHT_CURSOR));
				else
					wattron(grid, COLOR_PAIR(COLOR_HIGHLIGHT));
			}
			mvwprintw(grid, y, x, "%c", c);
			if (g_useHighlights && selected == c)
			{
				if (x == x_cursor && y == y_cursor)
					wattroff(grid, COLOR_PAIR(COLOR_HIGHLIGHT_CURSOR));
				else
					wattroff(grid, COLOR_PAIR(COLOR_HIGHLIGHT));
			}
			x += GRID_LINE_DELTA;
		}
		y += GRID_COL_DELTA;
	}
}

static void new_puzzle(void)
{
	int holes = get_holes(g_level);
	char* stream;

	if (g_provided_stream)
		stream = g_provided_stream;
	else
		stream = generate_puzzle(holes);

	//todo
	strcpy(plain_board, stream);
	strcpy(user_board, stream);

	if (!g_provided_stream)
		free(stream);

	fill_grid(plain_board, GRID_NUMBER_START_X, GRID_NUMBER_START_Y);

	g_playing = true;
}

static bool hint(void)
{
	char tmp_board[STREAM_LENGTH];
	int i, j, solved, try = 0;

	strcpy(tmp_board, user_board);
	solved = solve(tmp_board);
	if (solved != 0)
	{
		do
		{
			i = rand() % 8 + 1;
			j = rand() % 8 + 1;
			try++;
			if ( user_board[i*9+j] == '.')
			{
				user_board[i*9+j] = tmp_board[i*9+j];
				return true;
			}
		} while (try < MAX_HINT_RANDOM_TRY);
	}
	return false;
}

int main(int argc, char *argv[])
{
	bool run = true, enable_highlights=false;
	int key, x, y, posx, posy;

	g_provided_stream = NULL;

	parse_arguments(argc, argv);
	init_curses();
	init_windows();
	srand(time(NULL));

#ifdef DEBUG
	strcpy(plain_board, EXAMPLE_STREAM);
	strcpy(user_board, EXAMPLE_STREAM);
	fill_grid(plain_board, GRID_NUMBER_START_X, GRID_NUMBER_START_Y);
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
		// clear status window
		werase(status);
		switch(key)
		{
			case 'h':
			case KEY_LEFT:
				if(x>5)
				{
					x -= GRID_LINE_DELTA;
					if(g_playing)
					{
						// if we have highlighting enabled, we need to redraw the whole grid
						// so we can have the new colors in the matching colors.
						// this should only be done when we are playing, because plain_board
						// is actually the one being solved and thus displayed.
						// this is true for all movement keys.
						fill_grid(user_board, x, y);
					}
				}
				break;
			case 'l':
			case KEY_RIGHT:
				if(x<34)
				{
					x += GRID_LINE_DELTA;
					if(g_playing)
					{
						fill_grid(user_board, x, y);
					}
				}
				break;
			case 'k':
			case KEY_UP:
				if(y>2)
				{
					y -= GRID_COL_DELTA;
					if(g_playing)
					{
						fill_grid(user_board, x, y);
					}
				}
				break;
			case 'j':
			case KEY_DOWN:
				if(y<17)
				{
					y += GRID_COL_DELTA;
					if(g_playing)
					{
						fill_grid(user_board, x, y);
					}
				}
				break;
			case 'Q':
			case 27:
				run = false;
				break;
			case 'r':
			case KEY_RESIZE:
				redrawwin(grid);
				redrawwin(infobox);
				break;
			case 'S':
				if(g_playing)
				{
					g_useHighlights = false;
					werase(status);
					mvwprintw(status, 0, 0, "Solving puzzle...");
					refresh();
					wrefresh(status);
					solve(plain_board);
					fill_grid(plain_board, x, y);
					werase(status);
					mvwprintw(status, 0, 0, "Solved!");
					g_playing = false;
				}
				break;
			case 'N':
				g_useHighlights = false;
				werase(status);
				mvwprintw(status, 0, 0, "Generating puzzle...");
				refresh();
				wrefresh(status);
				new_puzzle();
				werase(status);
				g_playing = true;

				if (g_provided_stream)
				{
					free(g_provided_stream);
					g_provided_stream = NULL;
				}
				break;
			case 'c':
				if(g_playing)
				{
					int solvable;
					char tmp_board[STREAM_LENGTH];

					werase(status);

					strcpy(tmp_board, user_board);
					solvable= solve(tmp_board);

					if(solvable == 0)
					{
						mvwprintw(status, 0, 0, "Not correct");
					}
					else
					{
						if (strchr(user_board, '.') == NULL)
						{
							mvwprintw(status, 0, 0, "Solved!");
							g_playing = false;
						}
						else
						{
							mvwprintw(status, 0, 0, "Correct so far");
						}
					}
				}
				break;
			// delete
			case KEY_DC:
			case KEY_BACKSPACE:
			case 127:
			case 'x':
				if(g_playing)
				{
					posy = (y-GRID_NUMBER_START_Y)/GRID_COL_DELTA;
					posx = (x-GRID_NUMBER_START_X)/GRID_LINE_DELTA;
					// if on empty position
					if(plain_board[posy*9+posx] == '.')
					{
						user_board[posy*9+posx] = '.';
						wprintw(grid, " ");
					}
					break;
				}
			case 'H':
				if (g_playing && hint())
				{
					fill_grid(user_board, x, y);
					werase(status);
					mvwprintw(status, 0, 0, "Provided hint");
				}
				break;
			case 'M':
				g_useHighlights = !g_useHighlights;
				fill_grid(user_board, x, y);
				break;
			default:
				break;
		}
		/*if user inputs a number*/
		if(key >= 49 && key <= 57 && g_playing)
		{
			posy = (y-GRID_NUMBER_START_Y)/GRID_COL_DELTA;
			posx = (x-GRID_NUMBER_START_X)/GRID_LINE_DELTA;
			// if on empty position
			if(plain_board[posy*9+posx] == '.')
			{
				// add inputted number to grid
				user_board[posy*9+posx] = key;
				// redraw grid to update highlight
				fill_grid(user_board, x, y);
			}
		}
		wmove(grid, y,x);
		refresh();
		wrefresh(status);
		wrefresh(grid);
		wrefresh(infobox);
	}

	if (g_provided_stream)
		free(g_provided_stream);

	endwin();
	return EXIT_SUCCESS;
}


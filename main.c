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

$ gcc -lncurses -o nsudoku nsudoku.c
*/

/*
 * TODO:
 * read sudoku from CSV
 * save to CSV
 * hint (solve one number)
 * solve wholte sudoku
 * use vi keybindings (and arrows) -> nsudoku.c
 * use nice grid -> nsuds.c
 * two windows, one sudoku grind, one with description of keys
 * save to file not as CSV but the actual grid
 */

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* getopt */
#include <ncurses.h>

/* DEFINES */
#define VERSION		"0.1"
#define GRID_LINES	19
#define GRID_COLS	37
#define GRID_Y		3
#define GRID_X		3
#define INFO_LINES	25
#define INFO_COLS	20
#define INFO_Y		0
#define INFO_X		GRID_X + GRID_COLS + 5

/* GLOBALS */
bool g_useColor = true;
WINDOW *grid, *infobox;

/* FUNCTIONS */
void print_version(void)
{
	printf("nudoku version " VERSION "\n\n\
Copyright (C) Michael Vetter 2014\n\
License GPLv3+: GNU GPL version 3 or later.\n\
This is free software, you are free to modify and redistribute it.\n");
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
			init_pair(1, COLOR_YELLOW, COLOR_GREEN);
			init_pair(2, COLOR_RED, COLOR_BLUE);
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

	grid = newwin(GRID_LINES, GRID_COLS, GRID_Y, GRID_X);

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
}

int main(int argc, char *argv[])
{
	bool run = true;
	int key, x, y;

	parse_arguments(argc, argv);
	init_curses();

	init_windows();
	refresh();
	wrefresh(grid);
	wrefresh(infobox);

	y = 1; x = 2;
	wmove(grid, y, x);
	while(run)
	{
		//mvprintw(0, 0, "y: %d x: %d", y, x);
		refresh();
		wrefresh(grid);
		key = getch();
		switch(key)
		{
			case 'h':
				if(x>5)
					x -= 4;
				break;
			case 'l':
			case KEY_RIGHT:
				if(x<34)
					x += 4;
				break;
			case 'k':
				if(y>2)
					y -= 2;
				break;
			case 'j':
				if(y<17)
					y += 2;
				break;
			case 'q':
				run = false;
				break;
			case 'r':
				redrawwin(grid);
				redrawwin(infobox);
				break;
		}
		wmove(grid, y,x);
		refresh();
		wrefresh(grid);
		wrefresh(infobox);
	}

	endwin();
	return EXIT_SUCCESS;
}


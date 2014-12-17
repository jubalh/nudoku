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
#define VERSION "0.1"

/* GLOBALS */
bool g_useColor = true;

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

int main(int argc, char *argv[])
{
	parse_arguments(argc, argv);

	initscr();
	clear();

	if(g_useColor)
	{
		if(has_colors())
		{
			start_color();
		}
		else
		{
			printw( "Your terminal doesn't support colors.\nTry the nocolor (-c) option.\n");
			getch();
			exit(EXIT_FAILURE);
		}
	}

	refresh();
	getch();
	endwin();

	return EXIT_SUCCESS;
}


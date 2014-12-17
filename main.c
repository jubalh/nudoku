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
 */

/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* getopt */
#include <ncurses.h>

/* DEFINES */
#define VERSION "0.1"

/* GLOBALS */

/* FUNCTIONS */
void print_version(void)
{
	printf("nudoku version " VERSION "\n");
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
	while ((opt = getopt(argc, argv, "vhnl:")) != -1)
	{
		switch (opt)
		{
			case 'v':
				print_version();
				break;
			case 'h':
				print_usage();
				break;
			case 'l':
				printf("not yet implemented");
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
	printf("\n");
	return EXIT_SUCCESS;
}


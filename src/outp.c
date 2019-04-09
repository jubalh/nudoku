/*
vim: noexpandtab:ts=4:sts=4:sw=4

nudoku

Copyright (C) 2014 - 2019 Michael "jubalh" Vetter - jubalh _a-t_ iodoru.org

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
#include <cairo.h>
#include <stdio.h>

/* FUNCTIONS */
void generate_output(char *stream)
{
	cairo_surface_t *surface; cairo_t *cr;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 600, 600);
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	/*
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 10.0);

	cairo_move_to(cr, 5.0, 20.0); cairo_show_text(cr, "Nudoku");

	cairo_set_line_width(cr, 5);
	cairo_move_to(cr, 5.0, 70);
	cairo_line_to(cr, 455, 70);
	cairo_stroke(cr);
	*/

	cairo_set_line_width(cr, 2);
	for (int x=0; x < 9; x++)
	{
		for (int y=0; y < 9; y++)
		{
			cairo_rectangle(cr, 30 + x*50, 30 + y*50, 50, 50);
		}
	}
	cairo_stroke(cr);

	cairo_set_line_width(cr, 4);

	for (int x=0; x < 3; x++)
	{
		for (int y=0; y < 3; y++)
		{
			cairo_rectangle(cr, 30 + x*150, 30 + y*150, 150, 150);
		}
	}
	cairo_stroke(cr);

	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 25.0);
	int c;
	char s[4];
	for(int row=0; row < 9; row++)
	{
		for(int	col=0; col < 9; col++)
		{
			c = stream[row*9+col];
			if (c != '.')
			{
				cairo_move_to(cr, 45 + 50*row, 60 + 50*col);
				sprintf(s, "%c", c);
				cairo_show_text(cr, s);
			}
		}
	}

	cairo_surface_write_to_png(surface, "image.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

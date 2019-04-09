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

/* FUNCTIONS */
void generate_output(char *stream)
{
	cairo_surface_t *surface; cairo_t *cr;
	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 600, 600);
	cr = cairo_create(surface);

	cairo_set_source_rgb(cr, 0, 0, 0); cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 10.0);

	cairo_move_to(cr, 5.0, 20.0); cairo_show_text(cr, "Nudoku");

	cairo_set_line_width(cr, 5); cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);
	cairo_move_to(cr, 5.0, 70);
	cairo_line_to(cr, 455, 70);
	cairo_stroke(cr);

	cairo_surface_write_to_png(surface, "image.png");
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

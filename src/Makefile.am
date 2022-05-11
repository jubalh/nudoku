#AM_CFLAGS = --pedantic -Wall
#-DDEBUG

if HAVE_WIDE_NCURSES
nudoku_LDADD = ${ncursesw_LIBS}
else
nudoku_LDADD = ${ncurses_LIBS}
endif
bin_PROGRAMS = nudoku
nudoku_SOURCES = main.c sudoku.c sudoku.h

if BUILD_CAIRO
nudoku_SOURCES += outp.c outp.h
AM_CFLAGS := @CAIRO_CFLAGS@
nudoku_LDADD += @CAIRO_LIBS@
endif

AM_CPPFLAGS = -DLOCALEDIR=\""$(localedir)"\"
LIBS = @LIBINTL@

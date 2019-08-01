#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nudoku], [2.0.0], [jubalh@iodoru.org])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC
# Use C99.
AC_PROG_CC_C99

# Options
AC_ARG_ENABLE(
        [cairo],
        [AS_HELP_STRING([--enable-cairo], [enable cairo drawing @<:@default=no@:>@])], ,
        [enable_cairo="no"]
)

# Checks for libraries.

AM_CONDITIONAL([BUILD_CAIRO], [false])

if test "${enable_cairo}" = "yes"; then

        AM_CONDITIONAL([BUILD_CAIRO], [true])

        PKG_CHECK_MODULES([CAIRO], [cairo],, [AC_MSG_ERROR([
The cairo library was not found. Either install
the cairo development libraries, or compile without support (--disable-cairo)
])])
        AC_DEFINE([ENABLE_CAIRO], [1], [Enable cairo])

fi

have_wide_ncurses=no
AC_CHECK_LIB(ncursesw, initscr, [have_wide_ncurses=yes])
if test $have_wide_ncurses = no; then
   AC_CHECK_LIB(ncurses, initscr, [], [
   echo "nudoku requires ncurses"
   exit 1
   ])
fi
AM_CONDITIONAL([HAVE_WIDE_NCURSES], [test $have_wide_ncurses = yes])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Set gettext files
AM_GNU_GETTEXT_VERSION([0.19.6])
AM_GNU_GETTEXT_REQUIRE_VERSION([0.19.6])
AM_GNU_GETTEXT([external])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit strdup])

# Set config files
AC_CONFIG_FILES(Makefile src/Makefile po/Makefile.in)

#AC_OUTPUT
AC_OUTPUT()

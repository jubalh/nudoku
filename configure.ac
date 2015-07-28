#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([nudoku], [0.2.0], [jubalh@openmailbox.org])
AC_CONFIG_SRCDIR([src/main.c])
AM_INIT_AUTOMAKE()

# Checks for programs.
AC_PROG_CC
# Use C99.
AC_PROG_CC_C99

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit strdup])

#AC_OUTPUT
AC_OUTPUT(Makefile src/Makefile)

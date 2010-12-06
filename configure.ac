#
# Copyright (C) 2010 Andrey Mukhin
#

AC_PREREQ(2.59)
AC_INIT([libbloom], [0.1], [a.mukhin77@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([1.5 foreign])
AC_CONFIG_SRCDIR([include/fnv.hpp])
AC_CONFIG_HEADER([config.h])
AC_PREFIX_DEFAULT([/usr/local/libbloom])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AM_PROG_LIBTOOL

CPPFLAGS="${CPPFLAGS} -D_GNU_SOURCE -D__USE_GNU"

dnl Checks for header files.
AC_STDC_HEADERS
AC_CHECK_HEADERS(string.h sys/types.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile
                 tests/Makefile])
AC_OUTPUT

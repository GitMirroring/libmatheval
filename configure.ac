dnl Copyright (C) 1999, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2011,
dnl 2012, 2013, 2026 Free Software Foundation, Inc.
dnl
dnl This file is part of GNU libmatheval
dnl
dnl GNU libmatheval is free software: you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License as
dnl published by the Free Software Foundation, either version 3 of the
dnl License, or (at your option) any later version.
dnl
dnl GNU libmatheval is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of 
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with GNU libmatheval.  If not, see
dnl <http://www.gnu.org/licenses/>.

dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([libmatheval],[1.1.12],[bug-libmatheval@gnu.org])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([lib/matheval.h])
AC_CONFIG_HEADERS([config.h])

dnl Initialize the test suite.
AC_CONFIG_TESTDIR([tests])
AC_CONFIG_FILES([tests/matheval.sh], [chmod +x tests/matheval.sh])
AC_CONFIG_FILES([tests/Makefile])

dnl Checks for programs.
AC_PROG_CC
AM_PROG_LEX([noyywrap])
AC_PROG_YACC
LT_INIT
AM_CONDITIONAL([ENABLE_SHARED], [test "x$enable_shared" = xyes])

dnl Symbols internal to library are hidden by compiler, where supported.
AC_MSG_CHECKING([whether $CC supports -fvisibility=hidden])
save_CFLAGS=$CFLAGS
CFLAGS="$CFLAGS -fvisibility=hidden -Werror"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#pragma GCC visibility push(default)
extern int f (void);
#pragma GCC visibility pop
int g (void) { return 0; }]])],
  [have_visibility=yes], [have_visibility=no])
CFLAGS=$save_CFLAGS
AC_MSG_RESULT([$have_visibility])
CFLAG_VISIBILITY=
if test "x$have_visibility" = xyes; then
  CFLAG_VISIBILITY=-fvisibility=hidden
fi
AC_SUBST([CFLAG_VISIBILITY])
AM_CONDITIONAL([HAVE_VISIBILITY], [test "x$have_visibility" = xyes])
AM_MISSING_PROG([AUTOM4TE], [autom4te])

dnl Guile is needed by test suite only.
have_guile=no
PKG_PROG_PKG_CONFIG([0.9.0], [PKG_CONFIG=""])
if test -n "$PKG_CONFIG"; then
  PKG_CHECK_MODULES([GUILE], [guile-3.0], [have_guile=yes],
    [PKG_CHECK_MODULES([GUILE], [guile-2.2], [have_guile=yes],
      [PKG_CHECK_MODULES([GUILE], [guile-2.0], [have_guile=yes],
        [have_guile=no])])])
fi
AM_CONDITIONAL([HAVE_GUILE], [test "x$have_guile" = xyes])

dnl Checks for header files.
AC_CHECK_HEADERS([math.h strings.h stddef.h stdint.h xlocale.h])

dnl Numbers are read and written in C locale, set for current thread,
dnl if locale objects can be used with compiler flags given (feature
dnl test macros may hide them); otherwise decimal point is translated.
AC_CACHE_CHECK([for locale objects], [matheval_cv_locale_objects],
  [AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <locale.h>
#include <stdlib.h>
#if HAVE_XLOCALE_H
#include <xlocale.h>
#endif]], [[locale_t c_locale, old;
c_locale = newlocale (LC_NUMERIC_MASK, "C", (locale_t) 0);
old = uselocale (c_locale);
uselocale (old);
freelocale (c_locale);
return strtod ("1", NULL) != 1;]])],
    [matheval_cv_locale_objects=yes], [matheval_cv_locale_objects=no])])
if test "x$matheval_cv_locale_objects" = xyes; then
  AC_DEFINE([HAVE_LOCALE_OBJECTS], [1],
    [Define to 1 if locale objects (newlocale, uselocale, freelocale)
     can be used.])
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile doc/Makefile lib/Makefile libmatheval.pc])
AC_OUTPUT

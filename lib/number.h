/* Copyright (C) 2026 Free Software Foundation, Inc.

   This file is part of GNU libmatheval

   GNU libmatheval is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   GNU libmatheval is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU libmatheval.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef NUMBER_H
#define NUMBER_H 1

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>

/* Maximal length of number written, with terminating null character.  */
#define NUMBER_LENGTH 64

/* Numbers are read and written with "." as decimal point, whatever the
   locale of the program using the library.  */

/* Read number from string.  */
double number_read(const char *string);

/* Write number, in "%g" format, into string of given size, as
   snprintf() does.  Return length of number written, or that would be
   written if string is too short.  */
int number_format(char *string, size_t size, double number);

#endif

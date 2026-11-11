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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if HAVE_XLOCALE_H
#include <xlocale.h>
#endif
#include "number.h"
#include "xmalloc.h"

/* Read number from string written with "." as decimal point, in
   current locale, by translating "." to decimal point of locale.  */
static double read_translated(const char *string);

/* Replace decimal point of current locale with "." in string, and
   return string.  */
static char *point_to_dot(char *string);

double
number_read(const char *string)
{
#if HAVE_LOCALE_OBJECTS
	locale_t c_locale, old;	/* C locale and locale of thread.  */
	double number;	/* Number read.  */

	/* Read number in C locale, set for current thread only; if C
	   locale can't be created, translate decimal point instead.  */
	c_locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t) 0);
	if (c_locale) {
		old = uselocale(c_locale);
		number = strtod(string, NULL);
		uselocale(old);
		freelocale(c_locale);
		return number;
	}
#endif
	return read_translated(string);
}

int
number_format(char *string, size_t size, double number)
{
	char buffer[NUMBER_LENGTH];	/* Number written.  */
	int written;	/* Flag set when number written.  */
	int length;	/* Length of number written.  */
	size_t copied;	/* Number of characters copied.  */
#if HAVE_LOCALE_OBJECTS
	locale_t c_locale, old;	/* C locale and locale of thread.  */
#endif

	/* Write number in C locale, set for current thread only; if C
	   locale can't be created, translate decimal point instead.  */
	written = 0;
#if HAVE_LOCALE_OBJECTS
	c_locale = newlocale(LC_NUMERIC_MASK, "C", (locale_t) 0);
	if (c_locale) {
		old = uselocale(c_locale);
		snprintf(buffer, sizeof(buffer), "%g", number);
		uselocale(old);
		freelocale(c_locale);
		written = 1;
	}
#endif
	if (!written) {
		snprintf(buffer, sizeof(buffer), "%g", number);
		point_to_dot(buffer);
	}

	/* Copy only characters needed, as given size may be larger than
	   string actually is (strncpy() would pad up to size).  */
	length = strlen(buffer);
	if (size > 0) {
		copied =
		    (size_t) length < size ? (size_t) length : size - 1;
		memcpy(string, buffer, copied);
		string[copied] = 0;
	}
	return length;
}

static double
read_translated(const char *string)
{
	const char *point;	/* Decimal point of current locale.  */
	const char *dot;	/* Dot in string.  */
	char *copy;	/* String with decimal point of locale.  */
	size_t point_length;	/* Length of decimal point.  */
	size_t prefix;	/* Length of string before dot.  */
	double number;	/* Number read.  */

	point = localeconv()->decimal_point;
	point_length = strlen(point);
	dot = strchr(string, '.');
	if (!dot || point_length == 0 || !strcmp(point, "."))
		return strtod(string, NULL);

	/* Copy is sized for whole string, however long, with decimal
	   point of locale in place of dot.  */
	prefix = dot - string;
	copy = XMALLOC(char, strlen(string) + point_length);
	memcpy(copy, string, prefix);
	memcpy(copy + prefix, point, point_length);
	strcpy(copy + prefix + point_length, dot + 1);
	number = strtod(copy, NULL);
	XFREE(copy);
	return number;
}

static char *
point_to_dot(char *string)
{
	const char *point;	/* Decimal point of current locale.  */
	char *found;	/* Decimal point found in string.  */
	size_t length;	/* Length of decimal point.  */

	point = localeconv()->decimal_point;
	length = strlen(point);
	if (length == 0 || !strcmp(point, "."))
		return string;
	found = strstr(string, point);
	if (found) {
		*found = '.';
		memmove(found + 1, found + length,
			strlen(found + length) + 1);
	}
	return string;
}

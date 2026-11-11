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

/* Test that numbers are read and written with "." as decimal point,
   also when program uses locale with decimal comma, and that locale of
   program is not changed.  Test is skipped if no such locale is
   installed.  */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <matheval.h>

#define CHECK(condition) \
	if (!(condition)) { \
		fprintf(stderr, "%s:%d: check failed: %s\n", __FILE__, \
			__LINE__, #condition); \
		return EXIT_FAILURE; \
	}

int
main(void)
{
	/* Locales with decimal comma.  */
	const char *names[] = {
		"de_DE.UTF-8", "de_DE.utf8", "de_DE", "fr_FR.UTF-8",
		"fr_FR.utf8", "fr_FR", "en_DK.UTF-8", "en_DK.utf8"
	};
	const char *name;	/* Locale used.  */
	void *f, *d;	/* Evaluators.  */
	char number[80];	/* Long number.  */
	size_t i;	/* Loop counter.  */

	name = NULL;
	for (i = 0; i < sizeof(names) / sizeof(names[0]) && !name; i++)
		if (setlocale(LC_ALL, names[i])
		    && !strcmp(localeconv()->decimal_point, ","))
			name = names[i];
	if (!name) {
		printf("no locale with decimal comma, test skipped\n");
		return 77;
	}
	printf("locale: %s\n", name);

	f = evaluator_create("1.5*x+0.25");
	CHECK(f != NULL);
	CHECK(evaluator_evaluate_x(f, 2) == 3.25);
	CHECK(!strcmp(evaluator_get_string(f), "((1.5*x)+0.25)"));
	d = evaluator_derivative_x(f);
	CHECK(d != NULL);
	CHECK(evaluator_evaluate_x(d, 2) == 1.5);
	evaluator_destroy(d);
	evaluator_destroy(f);

	/* Number longer than any number written by library: 1 followed
	   by 70 zeros, times 10^-70.  */
	number[0] = '1';
	memset(number + 1, '0', 70);
	strcpy(number + 71, ".e-70");
	f = evaluator_create(number);
	CHECK(f != NULL);
	CHECK(evaluator_evaluate_x(f, 0) == 1);
	evaluator_destroy(f);

	/* Number from simplification.  */
	f = evaluator_create("5/2*x");
	CHECK(f != NULL);
	CHECK(!strcmp(evaluator_get_string(f), "(2.5*x)"));
	evaluator_destroy(f);

	/* Locale of program is unchanged.  */
	CHECK(!strcmp(localeconv()->decimal_point, ","));
	CHECK(!strcmp(setlocale(LC_NUMERIC, NULL), name));
	return EXIT_SUCCESS;
}

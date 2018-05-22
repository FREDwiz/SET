/*
 * bush--
 *
 * Programma sviluppato a supporto del laboratorio di
 * Sistemi di Elaborazione e Trasmissione del corso di laurea
 * in Informatica classe L-31 presso l'Universita` degli Studi di
 * Genova, anno accademico 2017/2018.
 *
 * Copyright (C) 2015-2017 by Giovanni Lagorio <giovanni.lagorio@unige.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void fail_errno(const char * const msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void *my_malloc(size_t size)
{
	void *result = malloc(size);
	if (!result)
		fail_errno("Cannot allocate memory with malloc");
	return result;
}

void *my_realloc(void *ptr, size_t size)
{
	void *result = realloc(ptr, size);
	if (!result)
		fail_errno("Cannot re-allocate memory");
	return result;
}

void yyerror(void *scanner, const struct shell * const sh, struct node ** const cmd, char const * const msg)
{
	fprintf (stderr, "Parsing error: %s\n", msg);
}

char *my_strdup(const char * const s) {
	char *result = strdup(s);
	if (!result)
		fail_errno("Cannot allocate memory for strdup");
	return result;
}

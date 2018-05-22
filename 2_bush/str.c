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

#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "str.h"

struct str {
	size_t len;
	char *chars;
};

struct str *str_new()
{
	struct str *s = my_malloc(sizeof(*s));
	s->len = 0;
	s->chars = my_malloc(1);
	*(s->chars) = '\0';
	return s;
}

void str_destroy(struct str *this)
{
	free(this->chars);
	free(this);
}

char *str_destroy_stealing_chars(struct str *this)
{
	char *chars = this->chars;
	free(this);
	return chars;
}

void str_append(struct str *this, const char *chars)
{
	size_t chars_len;
	if (!chars)
		return;
	chars_len = strlen(chars);
	this->chars = my_realloc(this->chars, this->len + chars_len + 1);
	strcpy(this->chars + this->len, chars);
	this->len += chars_len;
}


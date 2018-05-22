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

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

struct shell;
struct node;
void *my_malloc(size_t size);
void *my_realloc(void *ptr, size_t size);
char *my_strdup(const char * const s);
void fail_errno(const char * const msg);
void yyerror(void *scanner, const struct shell * const sh, struct node ** const cmd, char const * const msg);

#endif /* #ifndef UTILS_H */

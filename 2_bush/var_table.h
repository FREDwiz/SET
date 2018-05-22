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

#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdio.h>

struct var_table;
struct var_table *vt_new();
void vt_destroy(struct var_table * const this);
const char *vt_lookup(const struct var_table * const this, const char *const name);
void vt_set_value(struct var_table * const this, const char *const name, const char * const value);
void vt_dump_to_stdout(const struct var_table * const this);
char *vt_name_to_string(const struct var_table * const this, const char *const name);
char **vt_to_envp(const struct var_table * const this);

#endif /* #ifndef VARIABLES_H */


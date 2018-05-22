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

#ifndef SHELL_H
#define SHELL_H

#include "ast.h"

struct var_table;
struct shell;
struct shell *sh_new();
void sh_destroy(struct shell * const this);
const char *sh_get_prompt(const struct shell * const this);
struct var_table *sh_get_var_table(const struct shell * const this);
char **sh_get_environment(const struct shell * const this);
void sh_build_environment(struct shell * const this);
enum next_action sh_execute(struct shell * const this, const struct node * const cmd);

#define PATH "PATH"
#define PWD "PWD"
#define HOME "HOME"
#define PATH_SEPARATOR '/'
#define DIR_SEPARATOR ':'

#endif /* #ifndef SHELL_H */

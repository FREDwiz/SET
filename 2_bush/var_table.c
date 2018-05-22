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
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "var_table.h"
#include "utils.h"

struct var {
	char *name;
	char *value;
};

struct var_table {
	size_t len;
	size_t capacity;
	struct var *vars;
};

static struct var *vt_find_var(const struct var_table * const this, const char *const name)
{
	struct var *v = this->vars;
	int i;
	for(i=0; i<this->len; ++i, ++v)
		if (strcmp(v->name, name)==0)
			return v;
	return NULL;
}

void vt_dump_to_stdout(const struct var_table *const this)
{
	const struct var *v = this->vars;
	int i;
	const size_t len = this->len;
	for(i=0; i < len; ++i, ++v)
		printf("%s=%s\n", v->name, v->value);
}

char **vt_to_envp(const struct var_table * const this) {
	/* creare l'array con le variabili di ambiente, ricordarsi
	 * di terminare l'array con NULL */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
}

const char *vt_lookup(const struct var_table * const this, const char *const name)
{
	struct var *v = vt_find_var(this, name);
	if (v)
		return v->value;
	return NULL;
}

void vt_set_value(struct var_table * const this, const char * const name, const char * const value)
{
	struct var *v = vt_find_var(this, name);
	if (v)
		free(v->value);
	else {
		assert(this->len <= this->capacity);
		if (this->len == this->capacity) {
			this->capacity *= 2;
			this->vars = my_realloc(this->vars, this->capacity * sizeof(struct var));
		}
		v = this->vars + this->len++;
		v->name = my_strdup(name);
	}
	v->value = my_strdup(value);
}

struct var_table *vt_new()
{
	struct var_table *vt = my_malloc(sizeof(*vt));
	vt->len = 0;
	vt->capacity = 8;
	vt->vars = my_malloc(vt->capacity * sizeof(struct var));
	return vt;
}

void vt_destroy(struct var_table *this)
{
	int i;
	struct var *v = this->vars;
	for(i=0; i<this->len; ++i, ++v) {
		free(v->name);
		free(v->value);
	}
	free(this->vars);
	free(this);
}


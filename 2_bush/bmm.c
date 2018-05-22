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
#include <unistd.h>
#include "shell.h"
#include "var_table.h"
#include "utils.h"

void inject_environment_into_vartable(struct var_table *vt)
{
	extern char **environ;
	char **envp = environ;
	char *tmp;
	for(; *envp; ++envp) {
		const char * const current_var = *envp;
		char * const equal_sign = strchr(current_var, '=');
		size_t name_len;
		if (!equal_sign)
			continue;
		name_len = equal_sign - current_var;
		tmp = my_malloc(name_len + 1);
		strncpy(tmp, current_var, name_len);
		tmp[name_len] = 0;
		vt_set_value(vt, tmp, equal_sign + 1);
		free(tmp);
	}
}

void make_sure_PWD_is_set(struct var_table *vt)
{
	char *cwd;
	if (vt_lookup(vt, PWD))
		return;

	/* la variabile PWD non era presente; crearla e inizializzarla
	 * al valore restituito dalla system call getcwd() */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/

}

void make_sure_PATH_is_set(struct var_table *vt)
{
	if (!vt_lookup(vt, PATH))
		vt_set_value(vt, PATH, "/bin:/usr/bin");
}

void make_sure_HOME_is_set(struct var_table *vt)
{
	if (!vt_lookup(vt, HOME))
		vt_set_value(vt, HOME, "/");
}

int main()
{
	void lexer_loop(struct shell *sh); /* defined inside lexer.l */
	struct shell *sh = sh_new();
	struct var_table *vt = sh_get_var_table(sh);
	inject_environment_into_vartable(vt);
	make_sure_PWD_is_set(vt);
	make_sure_PATH_is_set(vt);
	make_sure_HOME_is_set(vt);
	sh_build_environment(sh);
	lexer_loop(sh);
	sh_destroy(sh);
	return EXIT_SUCCESS;
}



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

#include <sys/wait.h>
#include <errno.h>
#include "utils.h"
#include "shell.h"
#include "var_table.h"

struct shell {
	struct var_table *vars;
	char **environment;
};

struct shell *sh_new()
{
	struct shell *sh = my_malloc(sizeof(*sh));
	sh->vars = vt_new();
	sh->environment = NULL;
	return sh;
}

static void free_envp(char **envp)
{
	/* richiamare la funzione free per rilasciare la memoria
	 * allocata per l'ambiente envp */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
}

void sh_destroy(struct shell * const this)
{
	vt_destroy(this->vars);
	free_envp(this->environment);
	free(this);
}

struct var_table *sh_get_var_table(const struct shell * const this) {
	return this->vars;
}

const char *sh_get_prompt(const struct shell * const this)
{
    return "bush-- $ ";
}

char **sh_get_environment(const struct shell * const this)
{
	return this->environment;
}

void wait_for_termination_of_children()
{
	/* richiamare la system call wait() per controllare il valore
	 * di terminazione di tutti i processi precedentemente lanciati
	 * e segnalare se un processo termina con condizione di errore
	 * (cioè se l'exit_status è diverso da 0; usare WIFIEXITED
	 * e WEXITSTATUS per vedere se un processo e` terminato con exit
	 * ed estrarne lo stato) */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
}

void sh_build_environment(struct shell *this) {
	free_envp(this->environment);
	this->environment = vt_to_envp(this->vars);
}

enum next_action sh_execute(struct shell * const this, const struct node * const cmd) {
	if (cmd->are_redirections_ok(cmd, 1, 1)) {
		enum next_action result = cmd->execute(cmd, this, NO_REDIR, NO_REDIR);
		wait_for_termination_of_children();
		return result;
	}
	fprintf(stderr, "Invalid redirections.\n");
	return NA_CONTINUE;
}

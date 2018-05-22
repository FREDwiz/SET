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

#ifndef AST_H
#define AST_H

#include <stdlib.h>

struct shell;

enum next_action {
	NA_QUIT,
	NA_CONTINUE
};

#define NO_REDIR (-1)

struct node {
#ifdef DEBUG
	void (*dump_to_stdout)(const struct node * const this);
#endif /* #ifdef DEBUG */
	int (*are_redirections_ok)(const struct node * const this, int ok_in_redir, int ok_out_redir);
	enum next_action (*execute)(const struct node * const this, struct shell * const sh, int in_redir, int out_redir);
	void (*destroy)(struct node * const this);
	void *impl;
};

struct node *new_node_cd(char * const pathname);
struct node *new_node_quit();
struct node *new_node_show_variables();
struct node *new_node_set(char * const name, char * const value);
struct node *new_node_ext_cmd(char * const exec_name);
struct node *new_node_pipe(struct node * const left_cmd, struct node * const right_cmd);

void ext_cmd_add_arg(struct node * const this, char * const arg);
void ext_cmd_set_input_redirection(struct node * const this, char * const name);
void ext_cmd_set_output_redirection(struct node * const this, char * const name);

#endif


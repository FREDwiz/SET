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

#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "utils.h"
#include "var_table.h"
#include "shell.h"
#include "ast.h"

/* General utility and "member" functions */

static struct node *new_node_empty()
{
	struct node *n = my_malloc(sizeof(*n));
	memset(n, 0, sizeof(*n));
	return n;
}

static void node_destroy(struct node * const this)
{
	free(this->impl);
	free(this);
}

int nothing_to_check(const struct node * const this, int ok_in_redir, int ok_out_redir)
{
	return 1;
}

/*
 *
 * Built-in command @cd
 *
 */

struct cd_impl {
	char *pathname;
};

#ifdef DEBUG
void cd_dump_to_stdout(const struct node * const this)
{
	const struct cd_impl * const impl = this->impl;
	printf("@cd");
	if (impl->pathname)
		printf(" %s", impl->pathname);
}
#endif /* #ifdef DEBUG */

void cd_destroy(struct node * const this)
{
	struct cd_impl * const impl = this->impl;
	free(impl->pathname);
	node_destroy(this);
}

enum next_action cd_execute(const struct node * const this, struct shell * const sh, int in_redir, int out_redir)
{
	struct cd_impl * const impl = this->impl;
	assert(in_redir == NO_REDIR && out_redir == NO_REDIR);

	/* Implementazione del comando cd:
	 * - leggere il valore di pathname da impl, se NULL prendere come default il valore della variabile HOME;
	 * - richiamare la system call chdir();
	 * - gestire il cambiamento della variabile PWD */

/*** TO BE DONE START ***/
/*** TO BE DONE END ***/

	return NA_CONTINUE;
}

struct node *new_node_cd(char *const pathname)
{
	struct node *n = new_node_empty();
	struct cd_impl *impl;
#ifdef DEBUG
	n->dump_to_stdout = cd_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = nothing_to_check;
	n->execute = cd_execute;
	n->destroy = cd_destroy;
	impl = n->impl = my_malloc(sizeof(struct cd_impl));
	impl->pathname = pathname;
	return n;
}

/*
 *
 * Built-in command @quit
 *
 */

void quit_dump_to_stdout(const struct node * const this)
{
	printf("@quit");
}

enum next_action quit_execute(const struct node * const this, struct shell * const sh, int in_redir, int out_redir) {
	assert(in_redir == NO_REDIR && out_redir == NO_REDIR);
	return NA_QUIT;
}

struct node *new_node_quit()
{
	struct node *n = new_node_empty();
#ifdef DEBUG
	n->dump_to_stdout = quit_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = nothing_to_check;
	n->execute = quit_execute;
	n->destroy = node_destroy;
	return n;
}

/*
 *
 * Built-in command @show-variables
 *
 */

void show_variables_dump_to_stdout(const struct node * const this)
{
	printf("@show-variables");
}

enum next_action show_variables_execute(const struct node * const this, struct shell *const sh, int in_redir, int out_redir)
{
	assert(in_redir == NO_REDIR && out_redir == NO_REDIR);
	vt_dump_to_stdout(sh_get_var_table(sh));
	return NA_CONTINUE;
}

struct node *new_node_show_variables()
{
	struct node *n = new_node_empty();
#ifdef DEBUG
	n->dump_to_stdout = show_variables_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = nothing_to_check;
	n->execute = show_variables_execute;
	n->destroy = node_destroy;
	return n;
}

/*
 *
 * Built-in command @set
 *
 */

struct set_impl {
	char *name;
	char *value;
};

#ifdef DEBUG
void set_dump_to_stdout(const struct node * const this)
{
	struct set_impl *impl = this->impl;
	printf("@set %s = %s", impl->name, impl->value);
}
#endif /* #ifdef DEBUG */

void set_destroy(struct node * const this)
{
	struct set_impl *impl = this->impl;
	free(impl->name);
	free(impl->value);
	node_destroy(this);
}

enum next_action set_execute(const struct node * const this, struct shell * const sh, int in_redir, int out_redir)
{
	struct set_impl *impl = this->impl;
	assert(in_redir == NO_REDIR && out_redir == NO_REDIR);
	vt_set_value(sh_get_var_table(sh), impl->name, impl->value);
	sh_build_environment(sh);
	return NA_CONTINUE;
}

struct node *new_node_set(char *name, char *value)
{
	struct node *n = new_node_empty();
	struct set_impl *impl;
#ifdef DEBUG
	n->dump_to_stdout = set_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = nothing_to_check;
	n->execute = set_execute;
	n->destroy = set_destroy;
	impl = n->impl = my_malloc(sizeof(struct set_impl));
	impl->name = name;
	impl->value = value;
	return n;
}

/*
 *
 * External commands
 *
 */

struct ext_cmd_impl {
	size_t n_args;
	char **args; /* as usual, args[0] = executable name */
	char *in_redirection; /* NULL when no redirection asked */
	char *out_redirection; /* NULL when no redirection asked */
};

#ifdef DEBUG
void ext_cmd_dump_to_stdout(const struct node * const this)
{
	struct ext_cmd_impl *impl = this->impl;
	int i;
	char **a = impl->args;
	for(i=0; i<impl->n_args; ++i)
		printf("%s ", *a++);
	if (impl->in_redirection)
		printf("< %s ", impl->in_redirection);
	if (impl->out_redirection)
		printf("> %s ", impl->out_redirection);
}
#endif /* #ifdef DEBUG */

void ext_cmd_destroy(struct node * const this)
{
	struct ext_cmd_impl *impl = this->impl;
	int i;
	char **a = impl->args;
	for(i=0; i<impl->n_args; ++i)
		free(*a++);
	free(impl->args);
	free(impl->in_redirection);
	free(impl->out_redirection);
	node_destroy(this);
}

int ext_cmd_are_redirections_ok(const struct node * const this, int ok_in_redir, int ok_out_redir)
{
	struct ext_cmd_impl *impl = this->impl;
	return (ok_in_redir || impl->in_redirection == NULL) && (ok_out_redir || impl->out_redirection == NULL);
}

char *find_in_path(const char *path, const char *name)
{
	/* Se name contiene degli '/', restituirne una copia con strdup().
	 * Altrimenti, fare il parsing del path con strchr, per cercare il comando in tutte le directory.
	 * Usare la system call access() per verificare l'esistenza di un eseguibile
	 * corrispondente al comando digitato e, se lo si trova, restituire il path completo.
	 * In caso contrario, restituire NULL */

/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
	fprintf(stderr, "%s : command not found.\n", name);
	return NULL;
}

void redirect_fd(int from_fd, int to_fd)
{
	/* se from_fd non è NO_REDIR, redirezionare da from_fd a to_fd,
	 * usando la syscall dup2() */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
}

enum next_action ext_cmd_execute(const struct node * const this, struct shell * const sh, int in_redir, int out_redir)
{
	struct ext_cmd_impl *impl = this->impl;
	pid_t pid;
	assert(impl->in_redirection == NULL || in_redir == NO_REDIR);
	assert(impl->out_redirection == NULL || out_redir == NO_REDIR);
	char *const executable = find_in_path(vt_lookup(sh_get_var_table(sh), PATH), impl->args[0]);
	if (executable == NULL)
		goto cleanup_and_return;
	if (access(executable, X_OK)) {
		fprintf(stderr, "%s is not executable.\n", executable);
		goto cleanup_and_return;
	}
	if (impl->in_redirection && (in_redir = open(impl->in_redirection, O_RDONLY | O_CLOEXEC)) < 0)  {
		in_redir = NO_REDIR;
		perror(impl->in_redirection);
		goto cleanup_and_return;
	}
	if (impl->out_redirection && (out_redir = open(impl->out_redirection, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR)) < 0) {
		out_redir = NO_REDIR;
		perror(impl->out_redirection);
		goto cleanup_and_return;
	}
	/* Creare un processo figlio usando fork(); assegnare a pid il suo valore
	 * di ritorno, invocando fail_errno se la syscall fallisce */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
	if (pid == 0) {
		redirect_fd(in_redir, STDIN_FILENO);
		redirect_fd(out_redir, STDOUT_FILENO);
		/* usare execve per lanciare l'eseguibile, segnalando un errore (e terminando il processo figlio) se
		 * la syscall fallisce. */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/
	}
	cleanup_and_return:
	free(executable);
	if (in_redir != NO_REDIR && close(in_redir))
		perror("Cannot release the FD of input redirection");
	if (out_redir != NO_REDIR && close(out_redir))
		perror("Cannot release the FD of output redirection");
	return NA_CONTINUE;
}

struct node *new_node_ext_cmd(char *exec_name)
{
	struct node *n = new_node_empty();
	struct ext_cmd_impl *impl;
#ifdef DEBUG
	n->dump_to_stdout = ext_cmd_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = ext_cmd_are_redirections_ok;
	n->execute = ext_cmd_execute;
	n->destroy = ext_cmd_destroy;
	impl = n->impl = my_malloc(sizeof(struct ext_cmd_impl));
	impl->n_args = 1;
	impl->args = my_malloc(sizeof(char *) * 2);
	impl->args[0] = exec_name;
	impl->args[1] = NULL;
	impl->in_redirection = NULL;
	impl->out_redirection = NULL;
	return n;
}

void ext_cmd_add_arg(struct node *this, char *arg)
{
	struct ext_cmd_impl *impl = this->impl;
	size_t old_n = impl->n_args++;
	impl->args = my_realloc(impl->args, (old_n + 2) * sizeof(char *));
	impl->args[old_n] = arg;
	impl->args[old_n+1] = NULL;
}

void ext_cmd_set_input_redirection(struct node * const this, char * const name)
{
	struct ext_cmd_impl *impl = this->impl;
	impl->in_redirection = name;
}

void ext_cmd_set_output_redirection(struct node * const this, char * const name)
{
	struct ext_cmd_impl *impl = this->impl;
	impl->out_redirection = name;
}

/*
 *
 * Pipes
 *
 */

struct pipe_impl {
	struct node *left_cmd;
	struct node *right_cmd;
};

#ifdef DEBUG
void pipe_dump_to_stdout(const struct node * const this)
{
	struct pipe_impl *impl = this->impl;
	printf("<");
	impl->left_cmd->dump_to_stdout(impl->left_cmd);
	printf(" | ");
	impl->right_cmd->dump_to_stdout(impl->right_cmd);
	printf(">");
}
#endif /* #ifdef DEBUG */

void pipe_destroy(struct node * const this)
{
	struct pipe_impl *impl = this->impl;
	impl->left_cmd->destroy(impl->left_cmd);
	impl->right_cmd->destroy(impl->right_cmd);
	node_destroy(this);
}

int pipe_are_redirections_ok(const struct node * const this, int ok_in_redir, int ok_out_redir)
{
	struct pipe_impl *impl = this->impl;
	return impl->left_cmd->are_redirections_ok(impl->left_cmd, ok_in_redir, 0) &&
		 impl->right_cmd->are_redirections_ok(impl->right_cmd, 0, ok_out_redir);
}

enum next_action pipe_execute(const struct node * const this, struct shell * const sh, int in_redir, int out_redir)
{
	int pipes[2];
	int i;
	struct pipe_impl *impl = this->impl;

	/* richiamare la system call pipe() per inizializzare pipes[].
	 * Usare fcntl per fare in modo che i FD di pipes vengano chiusi automaticamente
	 * da una exec.
	 * Poi, fare in modo che l'output del comando left_cmd sia rediretto su pipes[1] e
	 * l'input del comando right_cmd su pipes[0] */
/*** TO BE DONE START ***/
/*** TO BE DONE END ***/

	return NA_CONTINUE;
}

struct node *new_node_pipe(struct node * const left_cmd, struct node * const right_cmd) {
	struct node *n = new_node_empty();
	struct pipe_impl *impl;
	assert(left_cmd);
	assert(right_cmd);
#ifdef DEBUG
	n->dump_to_stdout = pipe_dump_to_stdout;
#endif /* #ifdef DEBUG */
	n->are_redirections_ok = pipe_are_redirections_ok;
	n->execute = pipe_execute;
	n->destroy = pipe_destroy;
	impl = n->impl = my_malloc(sizeof(struct pipe_impl));
	impl->left_cmd = left_cmd;
	impl->right_cmd = right_cmd;
	return n;
}

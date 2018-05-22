%{
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
#include "utils.h"
#include "shell.h"
#include "var_table.h"
#include "ast.h"
%}

%define parse.lac full
%error-verbose
%define api.pure true
%lex-param {void *scanner}
%parse-param {void *scanner}
%parse-param {struct shell *sh}
%parse-param {struct node **cmd}

%union {
	char *str;
	struct node *node;
}

%{
int yylex(YYSTYPE *yylval_param, void *yyscanner);
%}


%token CD QUIT SET SHOW_VARIABLES
%token UNCLOSED_STRING_LITERAL RESERVED_KEYWORD NEWLINE
%token <str> IDENTIFIER NAME
%left '|'
%type <str> name
%type <node> command builtin_cmd pipes ext_cmd
%destructor { $$->destroy($$); } command builtin_cmd pipes ext_cmd
%destructor { free($$); } name IDENTIFIER NAME

%%

line:
| NEWLINE { *cmd = NULL; }
| command NEWLINE { *cmd = $1; }
;

command: /* a command can either be a built-in command or an external one */
  builtin_cmd
| pipes
;

builtin_cmd: /* built-in commands */
  CD { $$ = new_node_cd(NULL); }
| CD name { $$ = new_node_cd($2); }
| QUIT { $$ = new_node_quit(); }
| SHOW_VARIABLES { $$ = new_node_show_variables(); }
| SET IDENTIFIER '=' name { $$ = new_node_set($2, $4); }
;

pipes: /* non-empty sequences of ext_cmd, with optional redirections, separated by pipe operator */
  ext_cmd
| ext_cmd '<' name { ext_cmd_set_input_redirection($1, $3); $$=$1; }
| ext_cmd '>' name { ext_cmd_set_output_redirection($1, $3); $$=$1; }
| ext_cmd '<' name '>' name { ext_cmd_set_input_redirection($1, $3); ext_cmd_set_output_redirection($1, $5); $$=$1; }
| pipes '|' pipes { $$ = new_node_pipe($1, $3); }
;

ext_cmd: /* the invocation of an external command is just a non-empty sequences of names */
  name { $$ = new_node_ext_cmd($1); }
| ext_cmd name { ext_cmd_add_arg($$, $2); $$=$1; }
;

name:
  NAME { $$ = vt_name_to_string(sh_get_var_table(sh), $1); free($1); }
| IDENTIFIER
;


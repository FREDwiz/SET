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

#include <string.h>
#include <assert.h>
#include "utils.h"
#include "str.h"
#include "shell.h"
#include "var_table.h"
#include "ast.h"
#include "autogen_parser.h"

struct name_to_str_aux {
	const struct var_table *vt;
	struct str *str;
};
%}

%option noyywrap warn nodefault reentrant stack bison-bridge
%option extra-type="struct name_to_str_aux *"
%x NAME_TO_STRING

IDENTIFIER    [[:alpha:]_][[:alnum:]_]*
VAR_EXP_ID    "$"{IDENTIFIER}
VAR_EXP_CURLY "${"{IDENTIFIER}"}"
STR_LITERAL   "'"[^'\n]*"'"
ESCAPED_CHAR  "\\".
CHAR_SEQ      [^{}@$'\\#=<>|\n[:blank:]][^{}$'\\#=<>|\n[:blank:]]*
NAME          ({VAR_EXP_ID}|{VAR_EXP_CURLY}|{STR_LITERAL}|{ESCAPED_CHAR}|{CHAR_SEQ})+

%%

[[:blank:]]+|#.*   /* nop (=discards blanks and comments) */
\r?\n              return NEWLINE;
{IDENTIFIER}       yylval->str = my_strdup(yytext); return IDENTIFIER;
{NAME}             yylval->str = my_strdup(yytext); return NAME;
'[^'\n]*/\n        return UNCLOSED_STRING_LITERAL; /* not strictly needed, but produces better error messages */
@quit              return QUIT;
@cd                return CD;
@set               return SET;
@show-variables    return SHOW_VARIABLES;
@[^[:blank:]\n]*   return RESERVED_KEYWORD;
.                  return *yytext;

<NAME_TO_STRING>{VAR_EXP_ID}    str_append(yyextra->str, vt_lookup(yyextra->vt, yytext+1));
<NAME_TO_STRING>{VAR_EXP_CURLY} yytext[yyleng-1] = '\0'; str_append(yyextra->str, vt_lookup(yyextra->vt, yytext+2));
<NAME_TO_STRING>{STR_LITERAL}   yytext[yyleng-1] = '\0'; str_append(yyextra->str, yytext+1);
<NAME_TO_STRING>\\t             str_append(yyextra->str, "\t");
<NAME_TO_STRING>\\n             str_append(yyextra->str, "\n");
<NAME_TO_STRING>{ESCAPED_CHAR}  { char buf[2]; *buf = yytext[1]; buf[1] = 0; str_append(yyextra->str, buf); }
<NAME_TO_STRING>{CHAR_SEQ}      str_append(yyextra->str, yytext);
<NAME_TO_STRING>.|\n            printf("Unexpected char in NAME_TO_STRING: %s\n", yytext); assert(0);

%%

char *vt_name_to_string(const struct var_table * const vt, const char *const name)
{
	YY_BUFFER_STATE buf;
	yyscan_t scanner;
	struct str *s = str_new();
	struct name_to_str_aux aux = { vt, s };
	assert(vt);
	assert(name);
	yylex_init(&scanner);
	buf = yy_scan_string(name, scanner);
	yy_push_state(NAME_TO_STRING, scanner);
	yyset_extra(&aux, scanner);
	yylex(NULL, scanner);
	yy_pop_state(scanner);
	yy_delete_buffer(buf, scanner);
	yylex_destroy(scanner);
	return str_destroy_stealing_chars(s);
}

void lexer_loop(struct shell *sh)
{
	yyscan_t scanner;
	int must_quit = 0;
	if (yylex_init(&scanner))
		fail_errno("Cannot create the scanner");
	while (!must_quit) {
		struct node *cmd = NULL;
		char *line = NULL;
		size_t buf_size = 0;
		YY_BUFFER_STATE buf;
		printf("%s", sh_get_prompt(sh));
		fflush(stdout);
		if (getline(&line, &buf_size, stdin) < 0) {
			free(line);
			break;
		}
		buf = yy_scan_string(line, scanner);
		if (yyparse(scanner, sh, &cmd) == 0 && cmd) {
			/* printf("Parsed command: ");
			fflush(stdout);
			cmd->dump_to_stdout(cmd);
			printf("\n"); */
			must_quit = sh_execute(sh, cmd) == NA_QUIT;
			cmd->destroy(cmd);
		}
		free(line);
		yy_delete_buffer(buf, scanner);
	}
	yylex_destroy(scanner);
}



#CC=clang
CFLAGS=-g3 -O0 -Wall -pedantic # -DDEBUG
OBJS=lexer.o parser.o utils.o var_table.o shell.o str.o ast.o bmm.o
BIN_DIR=bin
BMM=$(BIN_DIR)/bmm
EXES=$(BMM)

.PHONY: clean tgz-students tgz-full

all: $(EXES)

$(BMM): $(OBJS) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BMM) $(OBJS)

ast.o: ast.c utils.h ast.h var_table.h shell.h
shell.o: shell.c utils.h shell.h ast.h var_table.h
str.o: str.c utils.h str.h
utils.o: utils.c utils.h
var_table.o: var_table.c var_table.h utils.h str.h
bmm.o: shell.h

parser.o: parser.y utils.h shell.h var_table.h ast.h
	bison -o autogen_parser.c -d parser.y
	$(CC) $(CFLAGS) -w -g3 -c -o parser.o autogen_parser.c

lexer.o: lexer.l parser.o utils.h str.h shell.h var_table.h ast.h
	flex -o autogen_lexer.c lexer.l
	$(CC) $(CFLAGS) -w -g3 -c -o lexer.o autogen_lexer.c

clean:
	rm -f $(OBJS) $(EXES) autogen_parser.c autogen_lexer.c autogen_parser.h
	rm -rf $(BIN_DIR)

tgz-students: clean
	cd .. ; tar cvzf bush_minus_minus-students.tgz --exclude=bush_minus_minus-students/bin --exclude=bush_minus_minus-students/.idea bush_minus_minus-students

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# tgz-full: clean
# 	cd .. ; tar cvzf bush_minus_minus-full.tgz --exclude=bush_minus_minus-full/bin --exclude=bush_minus_minus-full/.idea bush_minus_minus-full


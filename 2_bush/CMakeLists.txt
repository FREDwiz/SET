cmake_minimum_required(VERSION 3.3)
project(bush_minus_minus_full)

find_package(FLEX)
find_package(BISON)

FLEX_TARGET(autogen_lexer lexer.l  ${CMAKE_CURRENT_SOURCE_DIR}/autogen_lexer.c)
BISON_TARGET(autogen_parser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/autogen_parser.c)
ADD_FLEX_BISON_DEPENDENCY(autogen_lexer autogen_parser)

add_definitions(-DDEBUG)

set(SOURCE_FILES
    ast.c
    ast.h
    bmm.c
    shell.c
    shell.h
    str.c
    str.h
    utils.c
    utils.h
    var_table.c
    var_table.h)

add_executable(bmm ${SOURCE_FILES} ${BISON_autogen_parser_OUTPUTS} ${FLEX_autogen_lexer_OUTPUTS})


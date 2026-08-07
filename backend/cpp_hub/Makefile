CC = gcc
CFLAGS = -Wall -g -Isrc
SRC_DIR = src

PARSER_C = $(SRC_DIR)/parser/parser.tab.c
PARSER_H = $(SRC_DIR)/parser/parser.tab.h
LEXER_C  = $(SRC_DIR)/lexer/lex.yy.c

all: cpp_compiler

$(PARSER_C) $(PARSER_H): $(SRC_DIR)/parser/parser.y
	bison -d -o $(PARSER_C) $<

$(LEXER_C): $(SRC_DIR)/lexer/lexer.l
	flex -o $@ $<

cpp_compiler: $(PARSER_C) $(LEXER_C) $(SRC_DIR)/ast/ast.c $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/codegen/codegen.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PARSER_C) $(PARSER_H) $(LEXER_C)
	rm -f cpp_compiler cpp_compiler.exe
CC = gcc
LEX = win_flex
YACC = bison -d

SRC_DIR = src

all: cpp_compiler.exe

# Ensure parser files (parser.tab.c & parser.tab.h) are generated FIRST
parser.tab.c parser.tab.h: $(SRC_DIR)/parser/parser.y
	$(YACC) -o parser.tab.c $(SRC_DIR)/parser/parser.y

lex.yy.c: $(SRC_DIR)/lexer/lexer.l parser.tab.h
	$(LEX) $(SRC_DIR)/lexer/lexer.l

cpp_compiler.exe: parser.tab.h lex.yy.c parser.tab.c $(SRC_DIR)/ast/ast.c $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/codegen/codegen.c
	$(CC) lex.yy.c parser.tab.c $(SRC_DIR)/ast/ast.c $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/semantic/semantic.c $(SRC_DIR)/codegen/codegen.c -I. -I$(SRC_DIR) -o cpp_compiler.exe

clean:
	cmd /c del /f /q lex.yy.c parser.tab.c parser.tab.h cpp_compiler.exe
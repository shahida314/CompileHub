CC = gcc
LEX = win_flex
YACC = bison -y

SRC_DIR = src

all: compiler.exe

compiler.exe: lex.yy.c y.tab.c $(SRC_DIR)/ast/ast.c $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/semantic/semantic.c
	$(CC) lex.yy.c y.tab.c $(SRC_DIR)/ast/ast.c $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/semantic/semantic.c -o compiler.exe

lex.yy.c: $(SRC_DIR)/lexer/lexer.l y.tab.h
	$(LEX) $(SRC_DIR)/lexer/lexer.l

y.tab.c y.tab.h: $(SRC_DIR)/parser/parser.y
	$(YACC) -d $(SRC_DIR)/parser/parser.y

clean:
	cmd /c del /f /q lex.yy.c y.tab.c y.tab.h compiler.exe
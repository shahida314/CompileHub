CC = gcc
LEX = flex
YACC = bison -d

all: java_compiler.exe

java_compiler.exe: parser.tab.c lex.yy.c
	$(CC) parser.tab.c lex.yy.c -o java_compiler.exe

parser.tab.c parser.tab.h: src/parser/parser.y
	$(YACC) src/parser/parser.y -o parser.tab.c

lex.yy.c: src/lexer/lexer.l parser.tab.h
	$(LEX) -o lex.yy.c src/lexer/lexer.l

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h java_compiler.exe
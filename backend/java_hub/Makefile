CC = gcc
LEX = flex
YACC = bison

SRC_DIR = src
LEXER_SRC = $(SRC_DIR)/lexer/lexer.l
PARSER_SRC = $(SRC_DIR)/parser/parser.y

TARGET = java_compiler.exe

all: $(TARGET)

$(TARGET): lex.yy.c parser.tab.c
	$(CC) parser.tab.c lex.yy.c -o $(TARGET)

parser.tab.c parser.tab.h: $(PARSER_SRC)
	$(YACC) -d $(PARSER_SRC)

lex.yy.c: $(LEXER_SRC) parser.tab.h
	$(LEX) $(LEXER_SRC)

clean:
	del /f /q parser.tab.c parser.tab.h lex.yy.c $(TARGET) 2>nul || rm -f parser.tab.c parser.tab.h lex.yy.c $(TARGET)

test-valid: $(TARGET)
	.\$(TARGET) tests/valid.txt

test-invalid: $(TARGET)
	.\$(TARGET) tests/invalid.txt
CC = gcc
CFLAGS = -Wall -g -Ibuild -Isrc/lexer -Isrc/parser

SRC_DIR = src
BUILD_DIR = build

TARGET = $(BUILD_DIR)/cpp_compiler.exe

all: $(TARGET)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/parser.tab.c: $(SRC_DIR)/parser/parser.y | $(BUILD_DIR)
	bison -d -o $(BUILD_DIR)/parser.tab.c $(SRC_DIR)/parser/parser.y

$(BUILD_DIR)/lex.yy.c: $(SRC_DIR)/lexer/lexer.l $(BUILD_DIR)/parser.tab.c | $(BUILD_DIR)
	flex -o $(BUILD_DIR)/lex.yy.c $(SRC_DIR)/lexer/lexer.l

$(TARGET): $(BUILD_DIR)/parser.tab.c $(BUILD_DIR)/lex.yy.c \
    $(SRC_DIR)/ast/ast.c $(SRC_DIR)/semantic/semantic.c \
    $(SRC_DIR)/symbol_table/symbol_table.c $(SRC_DIR)/codegen/codegen.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
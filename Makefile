# Herramientas
BISON   := bison
FLEX    := flex
CXX     := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g -Iinclude	-Wno-free-nonheap-object

# Directorios
BUILD_DIR := build
SRC_DIR := src

# Archivos generados
PARSER_SRC := $(BUILD_DIR)/parser.tab.cpp
PARSER_HEADER := $(BUILD_DIR)/parser.tab.hpp
LEXER_SRC := $(BUILD_DIR)/lex.yy.cpp

# Fuentes del proyecto
SOURCES := $(SRC_DIR)/main.cpp
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

EXECUTABLE := hulk-compiler

.PHONY: all clean

all: $(BUILD_DIR) $(EXECUTABLE)

$(BUILD_DIR):
	mkdir -p $@  

$(EXECUTABLE): $(OBJECTS) $(PARSER_SRC:.cpp=.o) $(LEXER_SRC:.cpp=.o)
	$(CXX)	$(CXXFLAGS)	$^	-o	$@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX)	$(CXXFLAGS)	-c	$<	-o	$@

# Generar parser con prefijo TOK_
$(PARSER_SRC) $(PARSER_HEADER): $(SRC_DIR)/parser.y
	$(BISON)	-d	-o	$(PARSER_SRC)	--defines=$(PARSER_HEADER)	$<

# Generar lexer
$(LEXER_SRC): $(SRC_DIR)/lexer.l	$(PARSER_HEADER)
	$(FLEX)	-o	$@	$<

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.cpp
	$(CXX)	$(CXXFLAGS)	-c	$<	-o	$@

clean:
	rm	-rf	$(BUILD_DIR)	$(EXECUTABLE)
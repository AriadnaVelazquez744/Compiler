# Configuración del compilador
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Herramientas de compilación
LEX = flex
YACC = bison

# Directorios
BUILD_DIR = build
SRC_DIR = src
LEXER_DIR = lexer
PARSER_DIR = parser
AST_DIR = ast

# Archivos fuente
LEXER_SRC = $(LEXER_DIR)/hulk.l
PARSER_SRC = $(PARSER_DIR)/hulk.y

LEXER_OUT = $(BUILD_DIR)/lex.yy.c
PARSER_OUT = $(BUILD_DIR)/hulk.tab.c
PARSER_HEADER = $(BUILD_DIR)/hulk.tab.h

# Archivos compilados
OBJS = $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/ast.o $(BUILD_DIR)/main.o

# Ejecutable final
BIN = $(BUILD_DIR)/hulk_compiler

# Regla principal
all: build $(BIN)

# Compilación del lexer con Flex
$(LEXER_OUT): $(LEXER_SRC)
	$(LEX) -o $@ $<

# Compilación del parser con Bison
$(PARSER_OUT) $(PARSER_HEADER): $(PARSER_SRC)
	$(YACC) -d -o $(PARSER_OUT) $<

# Compilación de los archivos fuente
$(BUILD_DIR)/lexer.o: $(LEXER_OUT) $(LEXER_DIR)/lexer.cpp
	$(CXX) $(CXXFLAGS) -c $(LEXER_DIR)/lexer.cpp -o $@

$(BUILD_DIR)/parser.o: $(PARSER_OUT) $(PARSER_DIR)/parser.cpp
	$(CXX) $(CXXFLAGS) -c $(PARSER_DIR)/parser.cpp -o $@

$(BUILD_DIR)/ast.o: $(AST_DIR)/ast.cpp
	$(CXX) $(CXXFLAGS) -c $(AST_DIR)/ast.cpp -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $@

# Enlace final
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Regla de limpieza
clean:
	rm -rf $(BUILD_DIR)

# Creación de la carpeta build si no existe
build:
	mkdir -p $(BUILD_DIR)

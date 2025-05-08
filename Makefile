# === VARIABLES GLOBALES ===

# Herramientas
BISON   := bison
FLEX    := flex
CXX     := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g	-Isrc	-Isrc/ast	-Wno-free-nonheap-object
LDFLAGS += -lfl -lstdc++

# Directorios
BUILD_DIR := build
SRC_DIR := src
LEXER_DIR := $(SRC_DIR)/lexer
PARSER_DIR := $(SRC_DIR)/parser

# Archivos fuentes
LEX_SRC := $(LEXER_DIR)/lexer.l
YACC_SRC = $(PARSER_DIR)/parser.y
MAIN_SRC = $(SRC_DIR)/main.cpp

# Archivos generados por flex y bison
PARSER_SRC := $(BUILD_DIR)/parser.tab.cpp
PARSER_HEADER := $(BUILD_DIR)/parser.tab.hpp
LEXER_SRC := $(BUILD_DIR)/lex.yy.cpp

# Objetos explícitos
MAIN_OBJ = $(BUILD_DIR)/main.o
LEX_OBJ = $(BUILD_DIR)/lexer/lex.yy.o
YACC_OBJ = $(BUILD_DIR)/parser/parser.tab.o

# Detectar automáticamente todos los *.cpp de src/
CPP_SRC := $(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp")
CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRC))

# Fuentes del proyecto
#SOURCES := $(SRC_DIR)/main.cpp
#OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

OBJS := $(MAIN_OBJ) $(CPP_OBJ) $(YACC_OBJ) $(LEX_OBJ)
EXEC := hulk-compiler


# === TARGETS ===

all:	compile

compile:	$(BUILD_DIR)	$(EXEC)	
	@echo	"✅ Build completo. Ejecutable en $(EXEC)"

run:	compile
	@echo	"🚀 Ejecutando script.hulk..."
	@./$(EXEC)	

clean:
	rm	-rf	$(BUILD_DIR)	$(EXEC)
	@echo "🧹 Proyecto limpiado."

# === REGLAS DE COMPILACIÓN ===

$(BUILD_DIR):
	mkdir -p $@  

$(BUILD_DIR)/lexer:
	mkdir -p $@

$(BUILD_DIR)/parser:
	mkdir -p $@

# Generar parser con prefijo TOK_
$(PARSER_SRC) $(PARSER_HEADER): $(PARSER_DIR)/parser.y	|	$(BUILD_DIR)/parser
	$(BISON)	-d	-o	$(PARSER_SRC)	--defines=$(PARSER_HEADER)	-Wno-deprecated	$<

# Generar lexer
$(LEXER_SRC): $(LEXER_DIR)/lexer.l	$(PARSER_HEADER)	|	$(BUILD_DIR)/lexer
	$(FLEX)	-o	$@	$<

# Compilar archivos generados
$(LEX_OBJ): $(LEXER_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(YACC_OBJ): $(PARSER_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar main.cpp y otros .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX)	$(CXXFLAGS)	-c	$<	-o	$@

$(EXEC): $(OBJS) 
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)
	@echo	"✅ Compilación completa. Ejecutable en $(EXEC)"

# === META ===
.PHONY: all compile run clean

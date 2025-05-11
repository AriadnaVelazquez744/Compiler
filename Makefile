# === VARIABLES GLOBALES ===

# Herramientas
CXX     := g++
BISON   := bison
FLEX    := flex
LLVM	:= llvm-config

CXXFLAGS := -std=c++17 -Wall -Wextra -g	-Isrc	-Isrc/ast	-Wno-free-nonheap-object
LDFLAGS += -lfl -lstdc++

LLVM_CXXFLAGS 	:= $(filter-out -fno-exceptions, $(shell $(LLVM) --cxxflags))
LLVM_LDFLAGS	:= $(shell $(LLVM) --ldflags --libs all --system-libs)

# Directorios
BUILD_DIR := .build
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

OBJS := $(MAIN_OBJ) $(CPP_OBJ) $(YACC_OBJ) $(LEX_OBJ)

EXEC := hulk-compiler
INPUT_FILE := $(word 2, $(MAKECMDGOALS))
LLVM_IR := output.ll
CODE := output

# === TARGETS ===
all:	build

build:	$(BUILD_DIR)	$(EXEC)	
	@echo	"✅ Build completo. Ejecutable en $(EXEC)"

run: build $(LLVM_IR) $(CODE)
	@echo "🚀 Ejecutando programa..."
	@./$(CODE)
	@echo "🏁 Ejecución completada"

$(LLVM_IR): build
	@./$(EXEC) $(INPUT_FILE)
%:
	@:

$(CODE): $(LLVM_IR)
	@clang $< -o $@
	@echo "🔨 Generado ejecutable: $(CODE)"

# Modificar el clean
clean:
	rm -rf $(BUILD_DIR) $(EXEC) $(LLVM_IR) $(CODE)
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
	$(CXX)	$(CXXFLAGS)	$(LLVM_CXXFLAGS)	-c	$<	-o	$@

$(EXEC): $(OBJS) 
	$(CXX)	$(CXXFLAGS)	$(LLVM_CXXFLAGS)	-o	$(EXEC) $(OBJS)	$(LLVM_LDFLAGS)
	@echo	"✅ Compilación completa. Ejecutable en $(EXEC)"

# === META ===
.PHONY: all build run clean

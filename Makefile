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
LEXER_GENERATOR_DIR := $(SRC_DIR)/lexer_generator

# Archivos fuentes
# LEX_SRC := $(LEXER_DIR)/lexer.l
# YACC_SRC = $(PARSER_DIR)/parser.y
MAIN_SRC = $(SRC_DIR)/main.cpp

# Archivos generados por flex y bison
# PARSER_SRC := $(BUILD_DIR)/parser.tab.cpp
# PARSER_HEADER := $(BUILD_DIR)/parser.tab.hpp
# LEXER_SRC := $(BUILD_DIR)/lex.yy.cpp

# Objetos explícitos
MAIN_OBJ = $(BUILD_DIR)/main.o
# LEX_OBJ = $(BUILD_DIR)/lexer/lex.yy.o
# YACC_OBJ = $(BUILD_DIR)/parser/parser.tab.o

# Lexer generator
LEXER_GENERATOR_EXEC := lexer-generator
LEXER_GENERATOR_SRC := $(LEXER_GENERATOR_DIR)/codegen/main.cpp
LEXER_GENERATOR_OBJ := $(BUILD_DIR)/lexer_generator/codegen/main.o

# Lexer generator dependencies
LEXER_GENERATOR_DEPS := $(LEXER_GENERATOR_DIR)/core/TokenPattern.cpp \
                       $(LEXER_GENERATOR_DIR)/core/GeneratorConfig.cpp \
                       $(LEXER_GENERATOR_DIR)/core/LexerGenerator.cpp \
                       $(LEXER_GENERATOR_DIR)/parser/TokenFileParser.cpp \
                       $(LEXER_GENERATOR_DIR)/regex/RegexNode.cpp \
                       $(LEXER_GENERATOR_DIR)/regex/RegexParser.cpp \
                       $(LEXER_GENERATOR_DIR)/automata/NFA.cpp \
                       $(LEXER_GENERATOR_DIR)/automata/DFA.cpp \
                       $(LEXER_GENERATOR_DIR)/algorithms/NFABuilder.cpp \
                       $(LEXER_GENERATOR_DIR)/algorithms/DFABuilder.cpp \
                       $(LEXER_GENERATOR_DIR)/codegen/CodeGenerator.cpp

LEXER_GENERATOR_DEPS_OBJ := $(patsubst $(LEXER_GENERATOR_DIR)/%.cpp, $(BUILD_DIR)/lexer_generator/%.o, $(LEXER_GENERATOR_DEPS))

# Parser generator
PARSER_GENERATOR_EXEC := parser-generator
PARSER_GENERATOR_SRC := $(PARSER_DIR)/codegen/main_codegen.cpp
PARSER_GENERATOR_OBJ := $(BUILD_DIR)/parser/codegen/main_codegen.o

# Parser generator dependencies
PARSER_GENERATOR_DEPS := $(PARSER_DIR)/core/GrammarAugment.cpp \
                        $(PARSER_DIR)/core/LR1ItemSetBuilder.cpp \
                        $(PARSER_DIR)/core/LR1ParsingTables.cpp \
                        $(PARSER_DIR)/grammar/PrecedenceSetup.cpp \
                        $(PARSER_DIR)/grammar/GrammarNormalizer.cpp \
                        $(PARSER_DIR)/codegen/CodeGenerator.cpp

PARSER_GENERATOR_DEPS_OBJ := $(patsubst $(PARSER_DIR)/%.cpp, $(BUILD_DIR)/parser/%.o, $(PARSER_GENERATOR_DEPS))

# Detectar automáticamente todos los *.cpp de src/ (excluyendo tests, build artifacts, generators, and old lexer files)
# Remove the exclusions for parser/core/* and parser/grammar/* since we need these files
CPP_SRC := $(shell find $(SRC_DIR) -name "*.cpp" ! -name "main.cpp" ! -path "*/test/*" ! -path "*/test*.cpp" ! -path "*/build/*" ! -path "*/lexer_generator/*" ! -path "*/lexer_interpreter/*" ! -path "*/parser/codegen/*")
CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CPP_SRC))

# Funciones auxiliares en C
RUNTIME_SRC := $(SRC_DIR)/runtime/hulk_runtime.c
RUNTIME_OBJ := $(BUILD_DIR)/runtime/hulk_runtime.o

# Generated lexer files (will be created by lexer generator)
GENERATED_LEXER_HEADER := $(LEXER_DIR)/.build/Lexer.hpp
GENERATED_LEXER_SOURCE := $(LEXER_DIR)/.build/Lexer.cpp
GENERATED_LEXER_OBJ := $(BUILD_DIR)/lexer/.build/Lexer.o

# Generated token types files
GENERATED_TOKEN_TYPES_SOURCE := $(LEXER_DIR)/.build/TokenTypes.cpp
GENERATED_TOKEN_TYPES_OBJ := $(BUILD_DIR)/lexer/.build/TokenTypes.o

# Generated parser files (will be created by parser generator)
GENERATED_PARSER_GRAMMAR_PRODUCTIONS := $(PARSER_DIR)/.build/grammar_productions.hpp
GENERATED_PARSER_FIRST_SETS := $(PARSER_DIR)/.build/first_sets.hpp
GENERATED_PARSER_FOLLOW_SETS := $(PARSER_DIR)/.build/follow_sets.hpp
GENERATED_PARSER_LR1_ITEM_SETS := $(PARSER_DIR)/.build/lr1_item_sets.hpp
GENERATED_PARSER_PARSING_TABLES := $(PARSER_DIR)/.build/parsing_tables.hpp

# Remove the object file dependencies since these are now headers
# GENERATED_PARSER_GRAMMAR_PRODUCTIONS_OBJ := $(BUILD_DIR)/parser/.build/grammar_productions.o
# GENERATED_PARSER_FIRST_SETS_OBJ := $(BUILD_DIR)/parser/.build/first_sets.o
# GENERATED_PARSER_FOLLOW_SETS_OBJ := $(BUILD_DIR)/parser/.build/follow_sets.o
# GENERATED_PARSER_LR1_ITEM_SETS_OBJ := $(BUILD_DIR)/parser/.build/lr1_item_sets.o
# GENERATED_PARSER_PARSING_TABLES_OBJ := $(BUILD_DIR)/parser/.build/parsing_tables.o

# GENERATED_PARSER_OBJS := $(GENERATED_PARSER_GRAMMAR_PRODUCTIONS_OBJ) \
#                         $(GENERATED_PARSER_FIRST_SETS_OBJ) \
#                         $(GENERATED_PARSER_FOLLOW_SETS_OBJ) \
#                         $(GENERATED_PARSER_LR1_ITEM_SETS_OBJ) \
#                         $(GENERATED_PARSER_PARSING_TABLES_OBJ)

# Update OBJS to remove the generated parser object files
OBJS := $(MAIN_OBJ) $(CPP_OBJ) $(RUNTIME_OBJ) $(GENERATED_LEXER_OBJ) $(GENERATED_TOKEN_TYPES_OBJ)

EXEC := hulk-compiler
INPUT_FILE := $(word 2, $(MAKECMDGOALS))
LLVM_IR := hulk-low-code.ll
CODE := hulk-code

# === TARGETS ===
all:	build

build:	$(BUILD_DIR)	$(LEXER_GENERATOR_EXEC)	$(GENERATED_LEXER_HEADER)	$(GENERATED_LEXER_SOURCE)	$(PARSER_GENERATOR_EXEC)	$(GENERATED_PARSER_GRAMMAR_PRODUCTIONS)	$(EXEC)	
	@echo	"✅ Build completo. Ejecutable en $(EXEC)"

run: build $(LLVM_IR) $(CODE)

compile: run 
	
execute:
	@if [ ! -f "$(CODE)" ]; then \
		$(MAKE) compile; \
	fi
	@echo "🚀 Ejecutando programa..."
	@./$(CODE)
	@echo "🏁 Ejecución completada"

$(LLVM_IR): build
	@./$(EXEC) $(INPUT_FILE)
%:
	@:

$(CODE): $(LLVM_IR)	$(RUNTIME_OBJ)
	@clang	$<	$(RUNTIME_OBJ)	-lm	-o	$@
	@echo "🔨 Generado ejecutable: $(CODE)"

clean:
	rm -rf $(BUILD_DIR) $(EXEC) $(LLVM_IR) $(CODE) $(LEXER_GENERATOR_EXEC) $(LEXER_DIR)/.build $(PARSER_GENERATOR_EXEC) $(PARSER_DIR)/.build
	@echo "🧹 Proyecto limpiado."

# === REGLAS DE COMPILACIÓN ===

$(BUILD_DIR):
	mkdir -p $@  

$(BUILD_DIR)/lexer:
	mkdir -p $@

$(BUILD_DIR)/parser:
	mkdir -p $@

# Generar parser con prefijo TOK_
# $(PARSER_SRC) $(PARSER_HEADER): $(PARSER_DIR)/parser.y	|	$(BUILD_DIR)/parser
# 	$(BISON)	-d	-o	$(PARSER_SRC)	--defines=$(PARSER_HEADER)	-Wno-deprecated	$<

# Generar lexer
# $(LEXER_SRC): $(LEXER_DIR)/lexer.l	$(PARSER_HEADER)	|	$(BUILD_DIR)/lexer
# 	$(FLEX)	-o	$@	$<

# Compilar archivos generados
# $(LEX_OBJ): $(LEXER_SRC)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# $(YACC_OBJ): $(PARSER_SRC)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RUNTIME_OBJ): $(RUNTIME_SRC)
	@mkdir -p $(dir $@)
	gcc -Wall -O2 -c $< -o $@

# Compilar main.cpp y otros .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX)	$(CXXFLAGS)	$(LLVM_CXXFLAGS)	-c	$<	-o	$@

# Compilar lexer generator
$(LEXER_GENERATOR_OBJ): $(LEXER_GENERATOR_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar lexer generator dependencies
$(BUILD_DIR)/lexer_generator/%.o: $(LEXER_GENERATOR_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar parser generator
$(PARSER_GENERATOR_OBJ): $(PARSER_GENERATOR_SRC)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar parser generator dependencies
$(BUILD_DIR)/parser/%.o: $(PARSER_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilar generated lexer source file
$(GENERATED_LEXER_OBJ): $(GENERATED_LEXER_SOURCE) $(GENERATED_LEXER_HEADER)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

# Compilar generated token types source file
$(GENERATED_TOKEN_TYPES_OBJ): $(GENERATED_TOKEN_TYPES_SOURCE)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

# Compilar generated parser source files
# $(BUILD_DIR)/parser/.build/%.o: $(PARSER_DIR)/.build/%.cpp
# 	@mkdir -p $(dir $@)
# 	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS) 
	$(CXX)	$(CXXFLAGS)	$(LLVM_CXXFLAGS)	-o	$(EXEC) $(OBJS)	$(LLVM_LDFLAGS)
	@echo	"✅ Compilación completa. Ejecutable en $(EXEC)"

# Lexer generator
$(LEXER_GENERATOR_EXEC): $(BUILD_DIR) $(LEXER_GENERATOR_OBJ) $(LEXER_GENERATOR_DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(LEXER_GENERATOR_OBJ) $(LEXER_GENERATOR_DEPS_OBJ)
	@echo "🔧 Lexer generator compilado"
	@echo "📝 Generando código del lexer..."
	@./$(LEXER_GENERATOR_EXEC)
	@echo "✅ Código del lexer generado en $(LEXER_DIR)/.build"

# Parser generator
$(PARSER_GENERATOR_EXEC): $(BUILD_DIR) $(PARSER_GENERATOR_OBJ) $(PARSER_GENERATOR_DEPS_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(PARSER_GENERATOR_OBJ) $(PARSER_GENERATOR_DEPS_OBJ)
	@echo "🔧 Parser generator compilado"
	@echo "📝 Generando código del parser..."
	@cd $(PARSER_DIR)/codegen && ../../../$(PARSER_GENERATOR_EXEC)
	@echo "✅ Código del parser generado en $(PARSER_DIR)/.build"

# Ensure generated lexer files exist
$(GENERATED_LEXER_HEADER) $(GENERATED_LEXER_SOURCE): $(LEXER_GENERATOR_EXEC)
	@echo "📋 Verificando archivos generados del lexer..."
	@if [ ! -f "$(GENERATED_LEXER_HEADER)" ] || [ ! -f "$(GENERATED_LEXER_SOURCE)" ]; then \
		echo "❌ Archivos del lexer no encontrados. Ejecutando generador..."; \
		./$(LEXER_GENERATOR_EXEC); \
	fi

# Ensure generated parser files exist
$(GENERATED_PARSER_GRAMMAR_PRODUCTIONS): $(PARSER_GENERATOR_EXEC)
	@echo "📋 Verificando archivos generados del parser..."
	@if [ ! -f "$(GENERATED_PARSER_GRAMMAR_PRODUCTIONS)" ]; then \
		echo "❌ Archivos del parser no encontrados. Ejecutando generador..."; \
		cd $(PARSER_DIR)/codegen && ../../../$(PARSER_GENERATOR_EXEC); \
	fi

# Make other generated parser files depend on the first one
$(GENERATED_PARSER_FIRST_SETS) $(GENERATED_PARSER_FOLLOW_SETS) $(GENERATED_PARSER_LR1_ITEM_SETS) $(GENERATED_PARSER_PARSING_TABLES): $(GENERATED_PARSER_GRAMMAR_PRODUCTIONS)

# === META ===
.PHONY: all build run compile execute clean

# HULK Compiler

Compilador del lenguaje HULK

## Overview

This project implements a compiler for the **HULK** programming language, designed to parse, analyze, and generate executable code from HULK source files. The compiler is written in C++ and leverages tools like **Flex** (for lexical analysis), **Bison** (for parsing), and **LLVM** (for code generation). The project also includes a custom runtime in C for string and math operations.

---

## Compilation Pipeline

The process that HULK code follows in this project is as follows:

1. **Lexical Analysis (Lexer)**
   - Implemented in `src/lexer/lexer.l` using Flex.
   - Converts the input HULK source code into a stream of tokens.
   - Handles keywords, identifiers, literals, operators, and reports lexical errors.

2. **Syntax Analysis (Parser)**
   - Implemented in `src/parser/parser.y` using Bison.
   - Defines the grammar of the HULK language.
   - Builds an Abstract Syntax Tree (AST) from the token stream.

3. **AST Construction**
   - AST node classes are defined in `src/ast/AST.hpp`.
   - The parser constructs a tree of ASTNode objects representing the program structure.

4. **Semantic Analysis**
   - Implemented in `src/semantic/SemanticAnalyzer.cpp` and `SemanticAnalyzer.hpp`.
   - Traverses the AST to check for semantic errors (type checking, scope resolution, function signatures, etc.).
   - Uses a symbol table for variable/function/type tracking.

5. **Code Generation**
   - Implemented in `src/codegen/LLVMGenerator.cpp` and `LLVMGenerator.hpp`.
   - Walks the AST and emits LLVM IR code.
   - The IR is saved to `hulk-low-code.ll`.

6. **Runtime Support**
   - Some operations (e.g., string concatenation, math functions) are implemented in C (`src/runtime/hulk_runtime.c`).
   - The final executable links against this runtime.

7. **Executable Generation**
   - The LLVM IR is compiled to a native executable (`hulk-code`) using Clang, linking the runtime.

---

## Example Workflow (with Operand Example)

Suppose you have a simple HULK program that performs an arithmetic operation:

```hulk
print(3 + 4 * 2);
```

### 1. Write your HULK code

Save the above code in a file, for example, `operand_example.hulk`.

### 2. Build the Compiler

Use the Makefile to build all necessary components:

```sh
make build
```

### 3. Compile the HULK Code

Compile your HULK source file to generate LLVM IR and the executable:

```sh
make run operand_example.hulk
```

- This will build the project (if not already built), run the compiler on your file, and generate `hulk-low-code.ll` and `hulk-code`.

### 4. Execute the Program

Run the generated executable:

```sh
make execute
```

- This will execute `hulk-code` and display the output of your HULK program.

### 5. Clean the Project (Optional)

To remove all build artifacts:

```sh
make clean
```

---

## Project Structure

```
Compiler/
├── src/
│   ├── main.cpp                # Entry point: orchestrates the compilation pipeline
│   ├── lexer/lexer.l           # Flex lexer definition
│   ├── parser/parser.y         # Bison parser definition
│   ├── ast/AST.hpp             # AST node definitions
│   ├── semantic/               # Semantic analysis (symbol table, analyzer, errors)
│   ├── codegen/                # LLVM IR code generation
│   └── runtime/hulk_runtime.c  # C runtime for string/math support
├── Makefile                    # Build instructions
├── hulk-compiler               # The compiler executable (after build)
├── hulk-low-code.ll            # Generated LLVM IR (after compilation)
├── hulk-code                   # Final executable (after linking)
└── README.md                   # This file
```

---

## HULK Language Features

- **Types and Classes:** Custom types with attributes and methods, inheritance.
- **Functions:** Inline and block functions, nested functions.
- **Variables:** Mutable and immutable variables (`let`).
- **Control Flow:** `if`, `elif`, `else`, `for`, `while`, `range`.
- **Operators:** Arithmetic, boolean, string concatenation, etc.
- **Built-in Functions:** `print`, math functions (`sin`, `cos`, `sqrt`, etc.).
- **Special Values:** `True`, `False`, `Null`, `PI`, `E`.

---

## Programmatic Details

- **Lexer:** Handles tokenization, tracks line/column for error reporting.
- **Parser:** Builds AST, supports error recovery.
- **AST:** Rich node hierarchy for all language constructs.
- **Semantic Analyzer:** Checks types, scopes, inheritance, function signatures.
- **Code Generator:** Uses LLVM to emit IR, supports all major constructs.
- **Runtime:** Provides string and math utilities not natively supported by LLVM.

---

## Building and Running

- Requires: `g++`, `flex`, `bison`, `llvm`, `clang`
- Build everything: `make`
- Clean: `make clean`
- Run the full pipeline: `make execute`

---

## Example Output

For the operand example above, the output will be:

```
11
```

---

## Contributing

Feel free to open issues or pull requests for improvements, bug fixes, or new features!

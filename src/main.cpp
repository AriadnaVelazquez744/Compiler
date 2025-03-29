#include <iostream>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
// #include "../ast/ast.hpp"
// #include "../ir/ir.hpp"

extern FILE* yyin;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.hulk>\n";
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    yyin = file;
    yyparse();  // Llama al parser, que usa el lexer automáticamente
    fclose(file);

    std::cout << "Compilación exitosa.\n";
    return 0;
}

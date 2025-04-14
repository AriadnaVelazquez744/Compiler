#include <cstdio>

extern int yyparse();
extern FILE *yyin;

int main(int argc, char **argv) {
    const char* filename = "script.hulk";  // Default file
    
    if (argc >= 2) {
        filename = argv[1];  // Use provided filename
    }

    FILE *input_file = fopen(filename, "r");
    if (!input_file) {
        perror("Error opening file");
        return 1;
    }

    yyin = input_file;
    yyparse();
    fclose(input_file);

    return 0;
}
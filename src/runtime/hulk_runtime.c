#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h> 

// a + b → "ab"
char* hulk_str_concat(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b) + 1;
    char* res = malloc(len);
    strcpy(res, a);
    strcat(res, b);
    return res;
}

// a + " " + b → "a b"
char* hulk_str_concat_space(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b) + 2;
    char* res = malloc(len);
    strcpy(res, a);
    strcat(res, " ");
    strcat(res, b);
    return res;
}

// Compare two strings for equality
bool hulk_str_equals(const char* a, const char* b) {
    if (!a || !b) return false;  // Null safety
    return strcmp(a, b) == 0;
}

double hulk_rand() {
    return rand() / (double)RAND_MAX;
}

double hulk_log_base_checked(double x, double base) {
    if (x <= 0 || base <= 1) {
        fprintf(stderr, "Math Error: log(x, base) requires x > 0 and base > 1\n");
        exit(1);
    }
    return log(x) / log(base);
}
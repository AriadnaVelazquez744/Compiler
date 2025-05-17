#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

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

double hulk_log_base(double x, double base) {
    return log(x) / log(base);
}

double hulk_rand() {
    return rand() / (double)RAND_MAX;
}

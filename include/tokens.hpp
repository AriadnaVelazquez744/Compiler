#ifndef TOKENS_HPP
#define TOKENS_HPP

// Tipo para valores semánticos (puede ser una unión)
using YYSTYPE = int;

// Evita que Bison redefina YYSTYPE
#define YYSTYPE_IS_DECLARED 1

#endif
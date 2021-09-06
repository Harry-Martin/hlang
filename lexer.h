#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typedef enum
{
    TOK_UNKNOWN = 0,
    TOK_EOF,
    TOK_SEMI,
    TOK_STR_LIT,
    TOK_KEYWORD,
    TOK_NUMBER,
    TOK_IDENT,
} TOKEN_TYPE;

typedef struct
{
    const char *value;
    TOKEN_TYPE type;
} Token;

typedef struct
{
    const char *src;
    size_t src_size;
    size_t src_index;
} Lexer;

Lexer *lexer(const char *src, size_t src_size);
Lexer *lexer_from_file(const char *filepath);

Token *lexer_next_token(Lexer *lexer);
void print_token(Token *token);

#endif /* LEXER_H */

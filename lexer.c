#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

static char *src_from_file(const char *filepath)
{
    char *src;
    FILE *f = fopen(filepath, "r");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        rewind(f);
        src = calloc(1, (size + 1) * sizeof(char));
        fread(src, sizeof(char), size, f);
    }
    else
    {
        fprintf(stderr, "ERROR: failed to open file: %s\n", filepath);
        exit(1);
    }
    return src;
}

static char current_char(Lexer *lexer)
{
    return (char)lexer->src[lexer->src_index];
}

static void advance_lexer(Lexer *lexer)
{
    const char c = current_char(lexer);
    if (c != '\0')
    {
        if(c == '\n')
        {
            lexer->line_no += 1;
            lexer->c_no = 1;
        }
        lexer->src_index += 1;
        lexer->c_no += 1;
    }
}

static char peek_char(Lexer *lexer, size_t peek_dist)
{
    if (((lexer->src_index) + peek_dist) < lexer->src_size - 1)
    {
        return (char)lexer->src[lexer->src_index + peek_dist];
    }

    return current_char(lexer);
}

static Token *token(TOKEN_TYPE type, const char *value)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->value = value;
    tok->type = type;

    return tok;
}

void print_token(Token *token)
{
    const char *type_string;
    switch (token->type)
    {
    case TOK_EOF:
        type_string = "TOK_EOF";
        break;
    case TOK_SEMI:
        type_string = ";";
        break;
    case TOK_EQUAL:
        type_string = "=";
        break;
    case TOK_STR_LIT:
        type_string = "TOK_STR_LIT";
        break;
    case TOK_KEYWORD:
        type_string = "TOK_KEYWORD";
        break;
    case TOK_NUMBER:
        type_string = "TOK_NUMBER";
        break;
    case TOK_IDENT:
        type_string = "TOK_IDENT";
        break;
    default:
        type_string = "UNPRINTABLE";
        break;
    }
    if (token->value != NULL)
    {
        printf("<%s, \"%s\">\n", type_string, token->value);
    }
    else
    {
        printf("<%s>\n", type_string);
    }
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

static void skip_whitespace(Lexer *lexer)
{
    while (is_whitespace(current_char(lexer)))
    {
        advance_lexer(lexer);
    }
}
Lexer *lexer(const char *src, size_t src_size)
{
    Lexer *lexer = calloc(1, sizeof(Lexer));
    lexer->src = src;
    lexer->src_size = src_size;
    lexer->line_no = 1;
    lexer->c_no = 1;

    return lexer;
}

Lexer *lexer_from_file(const char *filepath)
{
    const char *src = src_from_file(filepath);
    size_t src_size = strlen(src);
    Lexer *lex = lexer(src, src_size);

    return lex;
}

char *parse_str_lit(Lexer *lexer)
{
    advance_lexer(lexer);
    size_t str_length = 0;
    const char *str_start = &lexer->src[lexer->src_index];

    while (current_char(lexer) != '"')
    {
        ++str_length;
        advance_lexer(lexer);
    }

    char *string;
    string = calloc(1, (str_length + 1) * sizeof(char));
    memcpy(string, str_start, str_length);

    return string;
}

static char *parse_alpha_numeric(Lexer *lexer)
{
    size_t start = lexer->src_index;
    char c = current_char(lexer);
    while (isalpha(c) || isdigit(c))
    {
        advance_lexer(lexer);
        c = current_char(lexer);
    }
    size_t word_length = lexer->src_index - start;
    char *word = calloc(1, (word_length + 1) * sizeof(char));
    memcpy(word, &lexer->src[start], word_length);

    return word;
}

static bool is_keyword(const char *word)
{
    const char *keywords[] = {"print"};
    for (size_t i = 0; i < (sizeof(keywords) / sizeof(char *)); ++i)
    {
        const char *keyword = keywords[i];
        if (strcmp(keyword, word) == 0)
        {
            return true;
        }
    }
    return false;
}

Token *lexer_next_token(Lexer *lexer)
{
    char c;
    char *value;
    Token *t = calloc(1, sizeof(Token));
    skip_whitespace(lexer);
    c = current_char(lexer);
    switch (c)
    {
    case '\0':
        t = token(TOK_EOF, NULL);
        break;
    case ';':
        t = token(TOK_SEMI, NULL);
        break;
    case '=':
        t = token(TOK_EQUAL, NULL);
        break;
    case '"':
        t = token(TOK_STR_LIT, parse_str_lit(lexer));
        break;
    }
    if (isalpha(c))
    {
        const char *word = parse_alpha_numeric(lexer);
        if (is_keyword(word))
        {
            t = token(TOK_KEYWORD, word);
        }
        else
        {
            t = token(TOK_IDENT, word);
        }
    }
    else if (isdigit(c))
    {
        t = token(TOK_NUMBER, ""); // parse_number(lexer)); TODO: implement
    }

    if (t->type == TOK_UNKNOWN)
    {
        fprintf(stderr, "ERROR: unexpected character %c at (%ld:%ld)\n", c, lexer->line_no, lexer->c_no);
        exit(1);
    }
    
    advance_lexer(lexer);

    return t;
}

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "lex.h"

typedef enum TOKEN_TYPES
{
    TOK_EOF = 0,
    TOK_KEYWORD,
    TOK_SEMI,
    TOK_EQUALS,
    TOK_STRING,
    TOK_IDENT,
} TokenType;

void display_usage()
{
    printf("Usage: hi <path>\n");
}

void show_token(const Tok *t)
{
    const char *str;
    switch (t->tok_type)
    {
    case TOK_EOF:
        str = "TOK_EOF";
        break;
    case TOK_KEYWORD:
        str = "TOK_KEYWORD";
        break;
    case TOK_SEMI:
        str = "TOK_SEMI";
        break;
    case TOK_EQUALS:
        str = "TOK_EQUALS";
        break;
    case TOK_STRING:
        str = "TOK_STRING";
        break;
    case TOK_IDENT:
        str = "TOK_IDENT";
        break;
    }

    switch (t->tok_type)
    {
    case TOK_EOF:
    case TOK_SEMI:
    case TOK_EQUALS:
        printf("<%s>\n", str); /* don't print token value if value is implicit in it's type. */
        break;
    default:
        printf("<%s, %s>\n", str, t->tok_value);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        const PatternCollection *pat_col =
            create_token_patterns(5, "print", TOK_KEYWORD, ";", TOK_SEMI, "=", TOK_EQUALS, "\"[^\"]*\"", TOK_STRING,
                                  "[a-zA-Z][a-zA-Z0-9_]*", TOK_IDENT);

        const char *src = file_to_str(argv[1]);

        Lex *l = create_lexer(pat_col, src);

        const Tok *t;
        while ((t = next_tok(l)))
        {
            show_token(t);
            skip_whitespace(l);
            if (t->tok_type == TOK_EOF)
            {
                break;
            }
        }
    }
    else
    {
        display_usage();
        fprintf(stderr, "ERROR: %s expects 1 argument but recieved %d\n", argv[0], argc - 1);
        exit(1);
    }
    return 0;
}

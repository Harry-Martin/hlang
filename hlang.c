#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

void display_usage()
{
    printf("Usage: hi <path>\n");
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        Lexer *lexer = lexer_from_file(argv[1]);
        Token *t;
        while ((t = lexer_next_token(lexer)))
        {
            print_token(t);
            if (t->type == TOK_EOF)
            {
                break;
            }
        }
    }
    else
    {
        printf("ERROR: hi expects 1 argument but recieved %d\n", argc - 1);
        display_usage();
        return -1;
    }

    return 0;
}

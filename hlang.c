#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
void display_usage()
{
    printf("Usage: hi <path>\n");
}

const char *get_source(const char *path)
{
    char *src = NULL;
    FILE *f = fopen(path, "r");
    if (f)
    {
        /* get the size of the file */
        size_t src_size;
        fseek(f, 0, SEEK_END);
        src_size = ftell(f);
        rewind(f);

        /* read file into src */
        src = malloc(src_size + 1);
        fread(src, 1, src_size, f);
        src[src_size] = '\0';
    }
    else
    {
        fprintf(stderr, "ERROR: failed to open file: %s\n", path);
    }
    return src;
}

bool char_in_string(const char c, const char *string)
{
    size_t string_index = 0;
    while (string[string_index] != '\0')
    {
        if (string[string_index] == c)
        {
            return true;
        }
        ++string_index;
    }
    return false;
}

char *substr_by_delim(const char *src, char **dest, const char *delims)
{
    size_t char_count = 1;
    char *offset = (char *)src;
    while (!char_in_string(*offset, delims))
    {
        ++offset;
        ++char_count;
    }
    *dest = (char *)malloc(char_count);
    memcpy(*dest, src, char_count);
    return ++offset;
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
            if(t->type == TOK_EOF){
                break;
            }
        }

        /*const char *src = get_source(argv[1]);*/

        /*char *word;*/
        /*char *rest = (char *)src;*/
        /*do*/
        /*{*/
        /*rest = substr_by_delim(rest, &word, " ");*/
        /*printf("%s\n", word);*/

        /*} while (*word != '\0' && *rest != '\0');*/
        /*return 0;*/
    }
    else
    {
        printf("ERROR: hi expects 1 argument but recieved %d\n", argc - 1);
        display_usage();
        return -1;
    }

    return 0;
}

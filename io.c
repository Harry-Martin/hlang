#include "io.h"

char *file_to_str(const char *filepath)
{
    char *src;
    FILE *f = fopen(filepath, "r");
    if (f)
    {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        rewind(f);
        src = (char*)calloc(1, (size + 1) * sizeof(char));
        fread(src, sizeof(char), size, f);
    }
    else
    {
        fprintf(stderr, "ERROR: failed to open file: %s\n", filepath);
        exit(1);
    }
    return src;
}

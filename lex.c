#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "lex.h"

const Pat *create_token_pattern(const char *regex, const uint tok_type)
{
    regex_t reg;
    if (regcomp(&reg, regex, 0) == 1)
    {
        fprintf(stderr, "Failed to compile REGEX : %s", regex);
        exit(1);
    }
    Pat *pattern = (Pat *)calloc(1, sizeof(Pat));
    pattern->regex = reg;
    pattern->tok_type = tok_type;

    return pattern;
}

const PatternCollection *create_token_patterns(size_t pat_count, ...)
{
    PatternCollection *out = (PatternCollection *)calloc(1, sizeof(PatternCollection));
    const Pat **patterns = (const Pat **)calloc(pat_count, sizeof(Pat *));

    va_list args;
    va_start(args, pat_count);

    size_t i = 0;
    while (i < pat_count)
    {
        char *regex = va_arg(args, char *);
        uint tok_type = va_arg(args, uint);
        patterns[i] = create_token_pattern(regex, tok_type);
        ++i;
    }

    va_end(args);

    out->patterns = patterns;
    out->pattern_count = pat_count;

    return out;
}

const Tok *create_token(const uint tok_type, const char *tok_value)
{
    Tok *t = (Tok *)calloc(1, sizeof(Tok));
    t->tok_type = tok_type;
    t->tok_value = tok_value;

    return t;
}

Lex *create_lexer(const PatternCollection *pattern_collection, const char *src)
{
    Lex *l = (Lex *)calloc(1, sizeof(Lex));
    l->curr = src;
    l->line = 1;
    l->col = 1;
    l->pattern_collection = pattern_collection;

    return l;
}

bool advance_lexer(Lex *l)
{
    if (*l->curr == '\n')
    {
        ++(l->line);
        l->col = 0;
    }

    if (*l->curr != '\0')
    {
        ++(l->col);
        ++(l->curr);
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * set unmatched line and col  = lexer pos,
 * keep reading the next char and building string,
 * if string matches a RE, set type = token_type and set match = true,
 * if match is true and string now doesnt have any matches, set value = string built - last char,
 * return token.
 * if no matches
 * until string has no matches, then
 */
const Tok *next_tok(Lex *l)
{
    const char *start_char = l->curr;
    size_t unmatched_line = l->line;
    size_t unmatched_col = l->col;
    char unmatched_char = *l->curr;

    size_t match_no = 0;
    uint token_type = 0;

    char *token_value = (char *)calloc(2, sizeof(char)); // allocating 2 chars for null terminator
    *token_value = *l->curr;

    while (*l->curr != '\0')
    {
        bool match = false;
        for (size_t i = 0; i < l->pattern_collection->pattern_count; ++i)
        {
            regmatch_t pmatch;
            if (string_matches_regex(token_value, l->pattern_collection->patterns[i]->regex))
            {
                match = true;
                ++match_no;
                token_type = l->pattern_collection->patterns[i]->tok_type;
                break;
            }
        }

        size_t len = strlen(token_value);
        if (!match && match_no > 0)
        {
            token_value[len-1] = '\0';
            break;
        }
        else
        {
            advance_lexer(l);
            token_value = realloc(token_value, (strlen(token_value) + 2) * sizeof(char));
            token_value[len] = *l->curr;
            token_value[len + 1] = '\0';
        }
    }

    if (match_no == 0 && *token_value != '\0')
    {
        fprintf(stderr, "Unexpected symbol \"%c\" at (%ld:%ld)\n", unmatched_char, unmatched_line, unmatched_col);
        exit(1);
    }
    return create_token(token_type, token_value);
}

void skip_whitespace(Lex *l)
{
    char c = *l->curr;
    while (c == ' ' || c == '\n' || c == '\t')
    {
        advance_lexer(l);
        c = *l->curr;
    }
}

bool string_matches_regex(const char *string, regex_t regex)
{
    regmatch_t pmatch;
    bool has_match = regexec(&regex, string, 1, &pmatch, 0) == 0;
    return has_match && pmatch.rm_eo - pmatch.rm_so == strlen(string);
}

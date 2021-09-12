#ifndef LEX_H
#define LEX_H

#include <regex.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Represents the relationship between a regular expression and the token it represents.
 */
typedef struct PATTERN
{
    regex_t regex; /* regular expression to match a particular token */
    uint tok_type; /* type of token to create when the regular expression matches */
} Pat;

typedef struct PATTERN_COLLECTION
{
    const Pat** patterns;
    size_t pattern_count;
} PatternCollection;

/**
 * Construct a pattern that matches token of `tok_type` and setup the
 * regex pointer for regular expression `regex`.
 */
const Pat *create_token_pattern(const char *regex, const uint tok_type);

/**
 * Construct and setup an array of patterns of size `pat_count`.
 * Variadic arguments should follow the following pattern:
 *          `<const char *regex>, <const uint tok_type>, ...`.
 */
const PatternCollection *create_token_patterns(size_t pat_count, ...);

/**
 * A single lexical token.
 */
typedef struct TOKEN
{
    uint tok_type;   /* type of the token */
    const char *tok_value; /* string value of the token */
} Tok;

/**
 * Construct a single token of type `tok_type` and value `tok_value`.
 */
const Tok *create_token(const uint tok_type, const char *tok_value);

/**
 * A lexical analyser state.
 */
typedef struct
{
    const char *curr;     /* current char to parse */
    size_t line;          /* line number currently being parsed */
    size_t col;           /* column number currently being parsed */
    const PatternCollection *pattern_collection; /* array of patterns used to parse the source file */
} Lex;

/**
 * Construct a lexical analyser where  `src` is the input text to parse,
 * and `patterns` define the lexical tokens and regular expressions to generate them.
 */
Lex *create_lexer(const PatternCollection *patterns, const char* src);

/**
 * Generate the next lexical token for lexer `l`.
 */
const Tok *next_tok(Lex *l);

/**
 * Advance the lexer to parse the next char. 
 */
bool advance_lexer(Lex *l);

/**
 * Advance the lexer until the current character is not considered whitespace
 */
void skip_whitespace(Lex *l);

/**
 * returns true if the whole string matches a regular expression 
 */
bool string_matches_regex(const char *string, regex_t regex);


#endif /* LEX_H */

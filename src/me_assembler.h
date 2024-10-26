#ifndef __ME_ASSEMBLER__
#define __ME_ASSEMBLER__

#include <stdlib.h>
#include "me_commons.h"
static size_t errorcount = 0;

typedef struct tokens
{
    char **p_sz_toks;
    size_t *p_u_col;
    size_t nstr;
} tok_t, *p_tok_t;

typedef struct custom_literal
{
    char *value;
    char *identifier_copy;
    size_t id, value_len;
    int value_type, value_pos;
} custlit_t, *p_custlit_t;

typedef struct assembler_error
{
    size_t line;
    size_t column;
    int code;
    char *file;
    char *msg;

} asmerr_t, *p_asmerr_t;

p_tok_t split_str_into_tokens(char *inp, char sep, size_t line);
void print_p_toks_st(p_tok_t token);

void assemble(char *dir, size_t size);
#endif
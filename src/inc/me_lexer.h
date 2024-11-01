#ifndef __ME_LEXER__
#define __ME_LEXER__

#include "me_commons.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct tokens
{
    char **p_sz_toks;
    size_t *p_u_col;
    size_t nstr;
} tok_t, *p_tok_t;

void print_p_toks_st(p_tok_t token);
p_tok_t malloc_p_toks_st();
void free_p_toks_st(p_tok_t *token);
void update_p_toks_st(p_tok_t ref_tok, size_t length);
p_tok_t split_str_into_tokens(char *inp, char sep, size_t line);

#endif
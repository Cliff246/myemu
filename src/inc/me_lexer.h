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
void free_p_toks_st(p_tok_t token);
void update_p_toks_st(p_tok_t ref_tok, size_t length);
p_tok_t split_str_into_tokens(char *inp, char sep);
void print_p_toks_string(p_tok_t token, bool offset);
p_tok_t copy_p_toks_st(p_tok_t ref);
p_tok_t split_p_toks_st(p_tok_t ref, size_t nsplit);
p_tok_t cut_p_toks_st(p_tok_t ref, size_t nstart, size_t nstop);
char *alloc_p_toks_string(p_tok_t ref, bool offset);

#endif
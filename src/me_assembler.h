#ifndef __ME_ASSEMBLER__
#define __ME_ASSEMBLER__

#include <stdlib.h>
#include "me_commons.h"

extern size_t errorcount;



typedef struct tokens
{
    char **p_sz_toks;
    size_t *p_u_col;
    size_t nstr;
} tok_t, *p_tok_t;

typedef struct reference
{
    size_t norgin;
    char *p_sz_key;
    int argtype;
}reference_t, *p_reference_t;

typedef struct assembler_error
{
    size_t line;
    size_t column;
    int code;
    char *file;
    char *msg;

} asmerr_t, *p_asmerr_t;

typedef struct context
{
    
    p_hashtable_t p_identifier_table;
    char *p_sz_fname;
    size_t nlines;
    p_tok_t *p_tokens
}context_t, *p_context_t;

typedef struct program
{
    char *p_program;
    size_t n_memorysize, n_increment, n_used;
}program_t, *p_program_t;

p_tok_t split_str_into_tokens(char *inp, char sep, size_t line);

//this is to set the copy equal to NULL
//may not be best idea
void free_p_toks_st(p_tok_t *token);
void print_p_toks_st(p_tok_t token);


void assemble(char *dir, size_t size);
#endif
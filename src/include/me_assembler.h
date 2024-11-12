#ifndef __ME_ASSEMBLER__
#define __ME_ASSEMBLER__

#include <stdlib.h>
#include "me_commons.h"
#include "me_lexer.h"

extern size_t errorcount;


typedef enum
{
    none,
    function,
    constant,
} section_type_t;

typedef enum 
{
    e_ptr,
    e_uint,
    e_sint,
    e_hex,
    e_char,
    e_error,
}argument_type_t;

typedef struct memorysegment
{
    size_t memstart, memstop;
    char *mem_bytes;
}memseg_t, *p_memseg_t;


typedef struct assembler_error
{
    size_t line;
    size_t column;
    int code;
    char *file;
    char *msg;

} asmerr_t, *p_asmerr_t;

typedef struct
{
    size_t len, allocd;
    section_type_t sectype;
    int initalized;
    int id;
    p_memseg_t sector;
    p_tok_t *toks;
    char *keystr;
} section_t, *p_section_t;

typedef struct reference
{
    p_section_t sect;
} reference_t, *p_reference_t;



typedef struct context
{
    p_hashtable_t p_identifier_table;
    char *p_sz_fname;
    size_t nlines;
    p_tok_t *p_tokens;
    size_t n_sections;
    p_section_t *p_sections;
}context_t, *p_context_t;

typedef struct program
{
    char *p_program;
    size_t n_memorysize, n_increment, n_used;
}program_t, *p_program_t;


//this is to set the copy equal to NULL
//may not be best idea

p_section_t new_section(section_type_t type, char *key);
void update_section(p_section_t section, p_tok_t tok);
void free_section(p_section_t section);

void free_memsegment(p_memseg_t memseg);
void *make_reference();
p_memseg_t make_memsegment(char *bytes, size_t start, size_t end);
p_reference_t get_reference(void *ptr);
// does not free key
int assemble(const char *dir, char **bytes);
void free_reference(void *ptr);
//void assemble(char *dir, size_t size);
void stage1(p_context_t context, p_program_t program);
void stage2(p_context_t context, p_program_t program);

#endif

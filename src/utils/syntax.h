#ifndef __SYNTAX_UTILS__
#define __SYNTAX_UTILS__

#include "commons.h"
#include "myemu.h"
#include "hashmap.h"
#include "shared_ptr.h"

typedef struct instsyntax
{
	int id;
	//base class
	char *base;
	//keyword
	char *keyword;
	
	size_t arg_count;
   	size_t modifiers_count;
	size_t labels_count;
	size_t base_binary_count;
	size_t offset_counts;
	//argument types defined
	char **arg_types;
	//modifiers involved 
	char **modifiers;
	//labels for completion syntax
	char **labels;
	//the base binary solution set
	char **base_binary;
	//offsets for binary as in the 
	size_t *offset;
}syntax_t, *p_syntax_t;

int get_id();


p_syntax_t create_syntax_t(char *base, char *keyword);



void add_syntax_argument_types(p_syntax_t syn, char **argtypes, size_t size);
void add_syntax_lables(p_syntax_t syn, char **labels, size_t size);
void add_syntax_base_binary(p_syntax_t syn, char **base_binary, size_t size);
void add_syntax_offsets(p_syntax_t syn, size_t *offsets, size_t size);

void add_syntax_modifiers(p_syntax_t syn, char **modifiers, size_t size);

int64_t add_to_syntax_map(p_hashtable_t table, p_syntax_t syn);

#include "inst.h"
bool compare_to_syntax(p_syntax_t syn, p_tok_t tok);

void free_syntax_ptr(void *syntax);



bool validate_asminst_is_valid(p_asminst_t asminst, p_syntax_t syntax );
char *convert_asminst_to_bytes(p_asminst_t asminst, p_syntax_t syntax );

#endif

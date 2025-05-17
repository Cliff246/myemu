#ifndef __UTILL_IO__
#define __UTILL_IO__

#include "commons.h"
#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>

size_t get_tokens(p_tok_t **reftok, char *dir);


p_hashtable_t read_hashtable_file(char *path);



#endif

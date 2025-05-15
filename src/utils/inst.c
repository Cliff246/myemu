#include "inst.h"
#include "myemu.h"
#include <stdio.h>

#include <cjson/cJSON.h>


#define JSON_STR(cj, key, out)                                  \
    do															\
	{                                                        	\
        cJSON *_f = cJSON_GetObjectItemCaseSensitive((cj), key);\
        if (!cJSON_IsString(_f) || !_f->valuestring) return -1; \
        (out) = strdup(_f->valuestring);                        \
    } while(0)






int convert_to_ninst(p_asminst_t inst)
{
	return 0;
}
p_asminst_t make_asminst(p_tok_t line)
{
	return NULL;
}
void free_asminst(p_asminst_t inst)
{
	return;
}

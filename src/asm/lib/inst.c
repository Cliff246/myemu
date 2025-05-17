#include "inst.h"

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

void print_range(char *range, size_t start, size_t stop, size_t size)
{
    if (size < stop)
    {
        return;
    }
    for (size_t i = start; i < stop; i++)
    {
        print_bin(range[i], 8, 0);
        printf(" %02lu  %04d ", i, range[i]);

        print_hex(range[i], 0);
        printf(" ");
        if (isprint(range[i]))
        {
            printf("|%c| ", range[i]);
        }
        else
        {
            printf("____");
        }

        if (range[i] >= 0 && range[i] < sizeof(str_instructions) / sizeof(*str_instructions))
        {
            printf(" %s", str_instructions[(size_t)range[(int)i]]);
        }
        printf("\n");
    }
}

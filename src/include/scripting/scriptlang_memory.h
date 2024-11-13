#ifndef __SCRIPTLANG_MEMORY__
#define __SCRIPTLANG_MEMORY__


typedef struct scriptlang_memory 
{
    unsigned int size;
    void *ptr;
    int id;
    struct scriptlang_memory *next, *prev;
}sl_memory_t, *p_sl_memory_t;


#endif
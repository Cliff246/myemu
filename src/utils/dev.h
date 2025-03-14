#ifndef __DEV__
#define __DEV__

#include "commons.h"
#include "myemu.h"

#define SYSTEM_TYPESTR_LEN 10
#define COMPONENT_TYPESTR_LEN 10

typedef struct component
{

    void *component_content; 
    void (*update_this_component)(struct component *, char *fmt, ...);
    void (*free_this_component)(struct component *);
    char comp_type[COMPONENT_TYPESTR_LEN + 1];

}component_t, *p_component_t;

void malloc_component(char *typestr, void (*free_fnptr)(p_component_t *));
void free_component(void *);
typedef struct system 
{
    p_hashtable_t table;
    int devices;
    char type[SYSTEM_TYPESTR_LEN + 1];
        
    
} system_t, *p_system_t;

p_system_t init_system();
void free_system(p_system_t sys);



#endif

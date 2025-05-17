#ifndef __DEV__
#define __DEV__

#include "commons.h"
#include "myemu.h"
#include "component.h"


#define SYSTEM_TYPESTR_LEN 10
typedef struct system
{
	
    p_hashtable_t table;
    p_shared_t head;

    int ndevices;
    char type[SYSTEM_TYPESTR_LEN + 1];


} system_t, *p_system_t;

p_system_t init_system(void);
void free_system(p_system_t sys);



#endif

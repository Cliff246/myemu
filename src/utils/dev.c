#include "dev.h"


void free_component(void *ptr)
{
    p_component_t comp = ptr;
    comp->free_this_component(ptr); 
    
}


p_system_t init_system()
{
    const int size=1000;
    p_system_t sys = (p_system_t)malloc(sizeof(system_t));
    if(!sys)
    {

    }

    p_hashtable_t table = new_hash_table(size, free_component);;
    sys->table = table; 
    sys->devices = 0;
    const char default_name[] = "NOT NAMED";
    memcpy(sys->type, default_name, ARY_LENGTH(default_name)); 
    return sys;
}

#include "component.h"

void free_component(void *ptr)
{
	if(!ptr)
		return;
    p_component_t comp = ptr;
    comp->free_this_component(ptr);

}


p_component_t malloc_component(char *typestr, void (*free_fnptr)(p_component_t *))
{

}
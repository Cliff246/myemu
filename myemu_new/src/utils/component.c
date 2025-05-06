#include "component.h"

void free_component(void *ptr)
{
    p_component_t comp = ptr;
    comp->free_this_component(ptr); 
    
}

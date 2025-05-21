#ifndef __MEMBER__
#define __MEMBER__

#include "myemu.h"
#include "dev.h"
#include "component.h"

typedef struct member
{
    //slave ptr
    p_component_t *attached;
    char *member_class;
    char *member_name;    

}member_t, p_member_t;



#endif

#ifndef __COMPONENT__
#define __COMPONENT__


#define COMPONENT_TYPESTR_LEN 10

#define COMPONENT_NAMESTR_LEN 10

typedef struct component
{
    //children list
    struct component *children;
    //parent component
    struct component *parent;
    //ptr to content str
    void *content_member;
    int *content_member_types;
    //specific functions
    void (*update_this_component)(struct component *, char *fmt, ...);
    void (*free_this_component)(struct component *);
    //component_type
    char comp_type[COMPONENT_TYPESTR_LEN + 1];
    char comp_name[COMPONENT_NAMESTR_LEN + 1];
    //length of content;
    int ncontent_member;
    //length of children list
    int nchildren;
}component_t, *p_component_t;

p_component_t malloc_component(char *typestr, void (*free_fnptr)(p_component_t *));
void free_component(void *);




#endif

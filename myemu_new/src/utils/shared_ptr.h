#ifndef __SHARED_PTR__
#define __SHARED_PTR__

#include <stdio.h>
#include <stdlib.h>

typedef struct shared_ptr
{
	void *ptr;
	void(*free_up)(void *ptr);
	int count;
}shared_t, *p_shared_t;

typedef void (*free_fn_ptr)(void *ptr);

p_shared_t new_shared_pointer(void *ptr, free_fn_ptr func);
void *get_shared_pointer(p_shared_t shptr);
p_shared_t retrieve_shared_pointer(p_shared_t shptr);
void release_shared_pointer(p_shared_t shptr);



#endif

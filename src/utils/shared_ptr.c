#include "shared_ptr.h"



p_shared_t new_shared_pointer(void *ptr, free_fn_ptr func )
{
	if(!ptr)
		return NULL;	
	p_shared_t shared = (p_shared_t)malloc(sizeof(shared_t));
	if(shared == NULL)
		return NULL;
	shared->ptr = ptr;
	shared->free_up = func;
	shared->count = 0;
	return shared;
}


void *get_shared_pointer(p_shared_t shptr)
{

	if(!shptr)
		return NULL;
	return shptr->ptr;

}
p_shared_t retrieve_shared_pointer(p_shared_t shptr)
{
	if(!shptr)
		return NULL;
	shptr->count++;
	return shptr;
}

void release_shared_pointer(p_shared_t shptr)
{
	if(!shptr)
		return;
	
	if(shptr->count - 1 <= 0)
	{
		shptr->free_up(shptr->ptr);
		free(shptr);

	}
	else
	{
		shptr->count--;
	}	
	return;
}


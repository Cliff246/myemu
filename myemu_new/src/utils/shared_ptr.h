#ifndef __SHARED_PTR__
#define __SHARED_PTR__

typedef struct shared_ptr
{
  void *ptr;
  int shared;
}shared_ptr_t, *p_shared_ptr_t;

p_shared_ptr_t new_shared_ptr(void *ptr);

void *retrieve_shared_ptr(p_shared_ptr_t sptr);
void release_shared_ptr(p_shared_ptr_t sptr);

void free_shared_ptr(p_shared_ptr_t sptr);

#endif

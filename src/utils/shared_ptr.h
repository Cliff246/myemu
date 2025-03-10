#ifndef __SHARED_PTR__
#define __SHARED_PTR__

struct shared_ptr {
  void *ptr;
  int shared;
};

struct shared_ptr *new_shared_ptr(void *ptr);

void *retrieve_shared_ptr(struct shared_ptr *sptr);
void release_shared_ptr(struct shared_ptr *sptr);

void free_shared_ptr(struct shared_ptr *sptr);

#endif

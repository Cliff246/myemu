#ifndef __HASHMAP__
#define __HASHMAP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>


size_t hash_to_position(int64_t hash, size_t pos);

typedef char *pstr_t;
typedef void (*free_hashtable_data_ptr)(void *);

typedef struct hash_element
{
	char *p_key;
	void *p_data;
	struct hash_element *p_next;
} hashelem_t, *p_hashelem_t;

typedef struct hash_table
{
	p_hashelem_t *p_tablelist;
	size_t tablesize;
	free_hashtable_data_ptr freedata;
} hashtable_t, *p_hashtable_t;

int64_t hash(const char *key);
p_hashtable_t new_hash_table(size_t size, free_hashtable_data_ptr freedata);
void free_hash_table(p_hashtable_t table);

int64_t addto_hash_table(p_hashtable_t table, const char *key, void *data);
int64_t add_element_to_hashtable(p_hashtable_t table, p_hashelem_t elem);
// copyies key string so memory overhead so whoop whoop, but also saftey
// also i am lazy
p_hashelem_t new_hash_element(const char *str, void *data);
// cringe cause this can have a next ptr and we would lose a bunch of memory by
// just getting rid of it so i will give it back if it exists
p_hashelem_t free_hash_element(p_hashelem_t element,
							   free_hashtable_data_ptr freedata);

p_hashelem_t get_from_hash_table(p_hashtable_t table, const char *key);
p_hashelem_t remove_from_hash_table(p_hashtable_t table, const char *key);
void *getdata_from_hash_table(p_hashtable_t table, const char *key);

void print_hash_table(p_hashtable_t table);
void print_hash_elem(p_hashelem_t elem);

#endif

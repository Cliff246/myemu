#include "hashmap.h"


size_t hash_to_position(int64_t hash, size_t pos)

{
	size_t ret = hash % pos;
	return ret;
}


p_hashtable_t new_hash_table(size_t size, free_hashtable_data_ptr freedata)
{
	p_hashtable_t table = (p_hashtable_t)calloc(1, sizeof(hashtable_t));
	if (table)
	{
		p_hashelem_t *list = (p_hashelem_t *)calloc(size, sizeof(p_hashelem_t));
		if (list)
		{
			// prob not needed but safer
			memset(list, 0, sizeof(p_hashelem_t) * size);
			table->tablesize = size;
			table->p_tablelist = list;
			table->freedata = freedata;
			return table;
		}
		else
		{
			printf("ALLOC ERROR %s:%d", __FILE__, __LINE__);
			exit(1);
		}
	}
	else
	{
		printf("ALLOC ERROR %s:%d", __FILE__, __LINE__);
		exit(1);
	}
}

void free_hash_table(p_hashtable_t table)
{
	if (table)
	{
		p_hashelem_t *ary = table->p_tablelist;
		for (size_t iter = 0; iter < table->tablesize; iter++)
		{
			p_hashelem_t cur = ary[iter];
			// holy fuck this seems dangerous
			if (cur == NULL)
			{
				continue;
			}
			while (cur)
			{
				cur = free_hash_element(cur, table->freedata);
			}
			if (cur)
				free_hash_element(cur, table->freedata);
		}
		free(table->p_tablelist);
		free(table);
	}
}

p_hashelem_t remove_from_hash_table(p_hashtable_t table, const char *key)
{
	if (table)
	{
		int64_t keyhash = hash(key);
		size_t position = hash_to_position(keyhash, table->tablesize);
		p_hashelem_t current = table->p_tablelist[position];
		if (current != NULL)
		{
			// in hashtable array
			if (strcmp(key, current->p_key) == 0)
			{

				if (current->p_next)
				{
					p_hashelem_t next = current->p_next;
					table->p_tablelist[position] = next;
					current->p_next = NULL;
					return current;
				}
				else
				{
					table->p_tablelist[position] = NULL;
					return current;
				}
			}
			// in hashtable linkedlist
			else
			{

				// loop through nexts

				for (p_hashelem_t prev = NULL; current; current = current->p_next)
				{
					if (strcmp(key, current->p_key) == 0)
					{
						if (current->p_next != NULL)
						{
							prev->p_next = current->p_next;
							current->p_next = NULL;
							return current;
						}
						else
						{
							prev->p_next = NULL;
							return current;
						}
					}
					prev = current;
				}

				return NULL;
			}
		}
		// not in table
		else
		{
			return NULL;
		}
	}
	else
	{
		printf("TABLE NOT DEFINED %s:%d", __FILE__, __LINE__);
		exit(1);
	}
}

p_hashelem_t new_hash_element(const char *str, void *data)
{
	if (str)
	{
		p_hashelem_t elem = (p_hashelem_t)malloc(sizeof(hashelem_t));
		if (elem)
		{
			// ew
			size_t len = strlen(str);
			char *copy = (char *)calloc(len + 1, sizeof(char));
			if (copy)
			{

				// i feel safe using strcpy cause really it should have broken earlier
				// if it fails
				strcpy(copy, str);
				elem->p_data = data;
				elem->p_key = copy;
				elem->p_next = NULL;
				return elem;
			}
		}
	}
	// any fail

	{
		printf("FUNCTION FAIL  %s:%d", __FILE__, __LINE__);
		exit(1);
	}
}
// this is cringe but if there is next in LinkedList then we will return it, if
// not well it will be null
p_hashelem_t free_hash_element(p_hashelem_t element,
							   free_hashtable_data_ptr freedata)
{
	if (element)
	{
		p_hashelem_t next = element->p_next;
		free(element->p_key);
		freedata(element->p_data);
		free(element);
		return next;
	}
	else
	{
		printf("PARAMATER IS NULL: WILL FREE ANYWAYS %s:%d", __FILE__, __LINE__);
		free(element);
		return NULL;
	}
}

int64_t add_element_to_hashtable(p_hashtable_t table, p_hashelem_t elem)
{
	if (table && elem)
	{
		int64_t keyhash = hash(elem->p_key);
		size_t position = hash_to_position(keyhash, table->tablesize);
		p_hashelem_t current = table->p_tablelist[position];
		if (current == NULL)
		{
			table->p_tablelist[position] = elem;
		}
		else
		{
			if (strcmp(elem->p_key, current->p_key) == 0)
			{
				free_hash_element(elem, table->freedata);
				return -1;
			}
			for (; current->p_next != NULL; current = current->p_next)
			{
				if (strcmp(elem->p_key, current->p_key) == 0)
				{
					free_hash_element(elem, table->freedata);
					return -1;
				}
			}
			current->p_next = elem;
		}
		return position;
	}
	else
	{
		printf("PARAMATER IS NULL %s:%d", __FILE__, __LINE__);
		exit(1);
	}
}

int64_t addto_hash_table(p_hashtable_t table, const char *key, void *data)
{
	if (table && key && data)
	{
		p_hashelem_t elem = new_hash_element(key, data);
		return add_element_to_hashtable(table, elem);
	}
	else
	{
		printf("PARAMATER IS NULL %s:%d", __FILE__, __LINE__);
		exit(1);
	}
}

p_hashelem_t get_from_hash_table(p_hashtable_t table, const char *key)
{
	if (table)
	{
		int64_t keyhash = hash(key);
		size_t position = hash_to_position(keyhash, table->tablesize);
		p_hashelem_t element = table->p_tablelist[position];
		if (element)
		{
			if (strcmp(element->p_key, key) == 0)
			{
				return element;
			}
			else
			{
				while (element)
				{
					if (strcmp(element->p_key, key) == 0)
					{
						return element;
					}
					element = element->p_next;
				}
				return NULL;
			}
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		printf("PARAMATER IS NULL %s:%d\n", __FILE__, __LINE__);
		exit(1);
	}
}

void *getdata_from_hash_table(p_hashtable_t table, const char *key)
{
	p_hashelem_t elem = get_from_hash_table(table, key);
	if (elem)
	{
		// printf("%s\n", elem->p_key);
		return elem->p_data;
	}
	else
	{
		// printf("key not in table%s:%d\n", __FILE__, __LINE__);
		// exit(1);
		return NULL;
	}
}

int64_t hash(const char *key)
{
	int64_t result = 0x0123456789abcdef;
	while (*key)
	{
		result ^= *(key++);
		result = result << 5;
	}
	return result;
}

void print_hash_elem(p_hashelem_t elem)
{
	printf("element: data:|%p| key:|%s| next:|%p|\n", elem->p_data, elem->p_key,
		   elem->p_next);
}

void print_hash_table(p_hashtable_t table)
{
	if (table)
	{
		for (int i = 0; i < table->tablesize; i++)
		{

			if (table->p_tablelist[i] != NULL)
			{
				p_hashelem_t cur = table->p_tablelist[i];
				for (; cur != NULL; cur = cur->p_next)
				{
					print_hash_elem(cur);
				}
			}
		}
	}
}





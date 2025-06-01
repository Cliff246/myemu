#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include "commons.h"

#define LL_TYPE_INLINE true
#define LL_TYPE_PTR false

typedef struct ll_node
{
	struct ll_node *next;
	
	union 
	{
		void *ptr_content;
		struct 
		{
			unsigned size;
			char bytes[];	
		}byte_content;
	};

}ll_node_t, *p_ll_node_t;




typedef struct linkedlist
{
	p_ll_node_t head;
	p_ll_node_t tail;

	size_t length;
	int (*get_stream_length)(void *);
	int (*convert_to_stream)(void *, char *); 
	void (*free_void_ptr)(void *data);
	bool type;


}linkedlist_t, *p_linkedlist_t;
//new's
	 
p_linkedlist_t new_linked_list_inline( 
		void (*free_data)(void *),
	   	int (*convert_to_stream)(void *, char *), 
		int (*get_stream_length)(void *));

p_linkedlist_t new_linked_list_ptr( void (*free_data)(void *));


p_ll_node_t new_linked_list_node(p_linkedlist_t ll, void *data);
//free
void free_linked_list(p_linkedlist_t ll);
void free_linked_list_node(p_linkedlist_t ll, p_ll_node_t node);
//remove
void remove_linked_list(p_linkedlist_t ll, int index);
p_ll_node_t remove_linked_list_node(p_linkedlist_t, int index);

//append and insert
void append_linked_list(p_linkedlist_t ll, void *data);
void insert_linked_list(p_linkedlist_t ll, int index, void *data);
void insert_linked_list_node(p_linkedlist_t ll, int index, p_ll_node_t node);
//index
p_ll_node_t index_node_linked_list(p_linkedlist_t ll, int index);
void *index_linked_list(p_linkedlist_t ll, int index);



#endif 

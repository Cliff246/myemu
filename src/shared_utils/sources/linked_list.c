
#include "commons.h"
#include "linked_list.h"

	p_ll_node_t new_linked_list_node(p_linkedlist_t ll, void *data)
	{
		if(ll->type == LL_TYPE_INLINE)
		{
			
			int length = ll->get_stream_length(data);
			//printf("length %d\n", length);
			if(length < 0)
			{
				errno = ERANGE;
				return NULL;
			}
			//assumes this is not to be free'd
			char *buffer = (char *)malloc(sizeof(char) * length);
			int return_size = ll->convert_to_stream(data, buffer);
			
			//printf("return_size %d\n", return_size);
			size_t size = length + sizeof(ll_node_t);
			p_ll_node_t node = (p_ll_node_t)malloc(size);
			if(!node)
			{
				errno = ENOMEM;
				return NULL;
			}
			node->next = NULL;
			//printf("node started\n");
			
			node->byte_content.size = (unsigned)length;
			memcpy(node->byte_content.bytes, buffer, length);
			//printf("copy bytes over\n");
			free(buffer);
			return node; 
		}
		else
		{
			
			p_ll_node_t node = (p_ll_node_t)malloc(sizeof(ll_node_t));
			if(!node)
			{
				errno = ENOMEM;
				return NULL;
			}
			node->ptr_content = data;
			node->next = NULL;
			return node;
		}
		
	}


void *index_linked_list(p_linkedlist_t ll, int index)
{
		//printf("index %d ll %p\n", index, ll);	
	if(ll == NULL)
	{
		errno = EINVAL;
		return NULL;

	}
	if(index < 0)
	{
		errno = EINVAL;
		return NULL;
	}
	p_ll_node_t node =  index_node_linked_list(ll, index); 
	
	
	if(ll->type == LL_TYPE_INLINE)
	{
		
		return (void *)node->byte_content.bytes;
	
	}
	else
	{
		return node->ptr_content;
	}
}

p_linkedlist_t new_linked_list_inline( void (*free_data)(void *), int (*convert_to_stream)(void *, char *), int (*get_stream_length)(void *))
{
	p_linkedlist_t ll = (p_linkedlist_t)malloc(sizeof(linkedlist_t));
	if(!ll)
	{
		errno = ENOMEM;
		return NULL;
	}
	ll->free_void_ptr = free_data;
	ll->convert_to_stream = convert_to_stream;
	ll->get_stream_length = get_stream_length;
	ll->head = NULL;
	ll->tail = NULL;
	ll->type = LL_TYPE_INLINE;
	ll->length = 0;
	return ll;

}

p_linkedlist_t new_linked_list_ptr( void (*free_data)(void *))
{
	p_linkedlist_t ll = (p_linkedlist_t)malloc(sizeof(linkedlist_t));
	if(!ll)
	{
		errno = ENOMEM;
		return NULL;
	}
	ll->free_void_ptr = free_data;
	ll->convert_to_stream = NULL;
	ll->get_stream_length = NULL;
	ll->head = NULL;
	ll->tail = NULL;
	ll->type = LL_TYPE_PTR;
	ll->length = 0;
	return ll;
}



static void free_helper(p_ll_node_t head, void (*free_tool)(void *), bool type)
{
	if(!head)
	{
		errno = EINVAL;
		return;
	}
	p_ll_node_t current = head;	
	if(type == LL_TYPE_INLINE)
	{	
		while(current->next != NULL)
		{
			p_ll_node_t node = current;
			current = current->next;
			free(node);
		}
	}
	else
	{
		if(free_tool == NULL)
		{
			errno = EINVAL;
			return;
		}	
		while(current->next != NULL)
		{
			p_ll_node_t node = current;
			current = current->next;
			void *copy = node->ptr_content;
			free(node);
			free_tool(copy);
		}
	}
	return;
}

p_ll_node_t index_node_linked_list(p_linkedlist_t ll, int index)
{
	//printf("length %zu\n", ll->length);
	if(index <= ll->length)
	{
		//printf("inside loop\n");
		if(!ll->head)
		{
			//printf("no head\n");
			return NULL;
		}
		p_ll_node_t current = ll->head;
		for(int i = 0; current != NULL; current = current->next, ++i)
		{
			if(i == index)
			{
				return current;
			}
		}
		return NULL;
	}
	return NULL;	
}	

	void free_linked_list(p_linkedlist_t ll)
{
	
	if(!ll)
	{	
		errno = EINVAL;
		return;
	}
	free_helper(ll->head, ll->free_void_ptr, ll->type);
	free(ll);		
}


void free_linked_list_node(p_linkedlist_t ll, p_ll_node_t node)
{
	if(!node || !ll)
	{
		return;

	}
	if(ll->type == LL_TYPE_PTR)
	{

		ll->free_void_ptr(node->ptr_content);
	}
	free(node);
}

void append_linked_list(p_linkedlist_t ll, void *data)
{
	insert_linked_list(ll, ll->length, data);			
}







void insert_linked_list(p_linkedlist_t ll, int index, void *data)
{
	
	if(!ll || index < 0)
	{
		errno = EINVAL;
		return;
	}
	p_ll_node_t node =	new_linked_list_node(ll, data);	
	//printf("node at %p", node);
	//printf("insert linked list node\n");	
	insert_linked_list_node(ll, index, node);	
}	

void insert_linked_list_node(p_linkedlist_t ll, int index, p_ll_node_t node)
{
	//printf("head: %p\n", ll->head);	
	//printf("index %d\n", index);
	if(index == 0)
	{
		//printf("index == 0\n");			
		node->next = ll->head;
		ll->head = node;
		if(ll->length == 0)
		{
			ll->tail = node;
		}	
		ll->length += 1;
		return;
	}
	else if(index >= ll->length)
	{
		//printf("index >= length\n");
		
		ll->tail->next = node;
		ll->tail = node;
		ll->length += 1;
		return;
	}
	else
	{
		//printf("index > 0 and < length\n");
		p_ll_node_t current = index_node_linked_list(ll, index - 1);
		//printf("current%p\n", current);
		if(current == NULL)
		{
			//printf("failed");
			return;
		}
		node->next = current->next;

		current->next = node;
		ll->length += 1;
		return;	
	}
}


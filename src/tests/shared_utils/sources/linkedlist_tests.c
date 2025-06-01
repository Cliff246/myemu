#include "tests_manager.h"
#include "linkedlist_tests.h"
#include "linked_list.h"


static void fill_range(int *range, size_t size, int multiplier, int offset)
{
	if(!range)
	{
		errno = EINVAL;
		perror("error range");
		exit(EXIT_FAILURE);
	}
	
	for(size_t i = 0; i < size; ++i)
	{
		range[i] = i * multiplier + offset;
	}

}


static void free_data_ptr(void *ptr)
{
	free(ptr);
}




TEST(tests_linked_list)
{
	p_linkedlist_t ll = new_linked_list_ptr(free_data_ptr);
	const size_t size = 10;
	int *data1 = (int *)malloc(sizeof(int) * size);
	
	int *data2 = (int *)malloc(sizeof(int) * size);
	int *data3 = (int *)malloc(sizeof(int) * size);
	if(data1 == NULL || data2 == NULL)
	{
		errno = ENOMEM;
		perror("error malloc failed");
		exit(EXIT_FAILURE);
	}
	//fill ranges
	fill_range(data1, size, 1, 0);
	fill_range(data2, size, -1, 0);
	fill_range(data3, size, 1, size);	
	//insert linked list tests
	insert_linked_list(ll, 0, data1);
	insert_linked_list(ll, 1, data2);

	insert_linked_list(ll, 1, data3);
		
	
		

	return 0;
}

TEST(tests_append_linked_list)
{

	p_linkedlist_t ll = new_linked_list_ptr(free_data_ptr);
	const size_t size = 10;
	int *data1 = (int *)malloc(sizeof(int) * size);
	
	int *data2 = (int *)malloc(sizeof(int) * size);
	int *data3 = (int *)malloc(sizeof(int) * size);
	if(data1 == NULL || data2 == NULL)
	{
		errno = ENOMEM;
		perror("error malloc failed");
		exit(EXIT_FAILURE);
	}
	//fill ranges
	fill_range(data1, size, 1, 0);
	fill_range(data2, size, -1, 0);
	fill_range(data3, size, 1, size);	
	//insert linked list tests
	append_linked_list(ll, data1);
	append_linked_list(ll, data2);
	append_linked_list(ll, data3);
	
	if(ll->head->next->ptr_content != (void *)data2)
	{
		return 1;
	}
	if(ll->head->next->next->ptr_content != (void *)data3)
	{
		return 1;
	}

	if(ll->head->next->next != ll->tail)
	{
		return 1;
	}
	return 0;	
}


TEST_COLLECTOR(linkedlist)
{
	REGISTER_TEST(tests_linked_list, NOSKIP); 
	REGISTER_TEST(tests_append_linked_list, NOSKIP);
}




#include "myemu.h"
#include "commons.h"
#include "tokenizer.h"
#include "test.h"
#include "modules.h"
#include "device_descriptor.h"
#include "assert.h"
#include "errno.h"
#include "tests_manager.h"

int getdata(void *ptr)
{
    int *iptr = (int *)ptr;
    return *iptr;
}

void *makedata(int x)
{
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = x;
    return (void *)ptr;
}

void freedata(void *data)
{
    free(data);
}

void test1()
{


    const char *exclude = "l*f";
    p_tok_t token = split_str_into_tokens("***hello*****world*game*o**********", exclude);
    p_tok_t token2 = split_str_into_tokens("test", exclude);
    p_tok_t token3 = split_str_into_tokens("*", exclude);
    p_tok_t token4 = split_str_into_tokens("", exclude);
    p_tok_t token5 = split_str_into_tokens("*saf*", exclude);
    print_p_toks_st(token3);
    print_p_toks_st(token5);

    print_p_toks_st(token);
    p_tok_t split = split_p_toks_st(token, 1);
    print_p_toks_st(split);
    p_tok_t cutout = cut_substr_p_tok_t(token, "*");
    p_tok_t cutout2 = cut_substr_p_tok_t(cutout, "l");
    free(cutout);
    print_p_toks_st(cutout2);
    print_p_toks_st(token2);

    update_p_toks_st(token2, 10);
    print_p_toks_st(token2);
}


void test2()
{
    char buffer[1000];
    size_t size = getstr_with_cut_chars(buffer, 1000,"@a$top", "@$");
    DPRINTF("%s %d\n", buffer, size);
}

void testsuite()
{

    p_tok_t token = split_str_into_tokens("hello world ghost ghost", " ");
    print_p_toks_st(token);
    // p_hashtable_t table = new_hash_table(1000, freedata);
    //
    // addto_hash_table(table, "hello world", makedata(10));
    // addto_hash_table(table, "test", makedata(123));
    // int i1 = getdata(getdata_from_hash_table(table, "hello world"));
    // int i2 = getdata(getdata_from_hash_table(table, "test"));
    // printf("%d %d\n", i1, i2);
    //
    //
    // print_hash_table(table);
    //
    //
    // p_hashelem_t elem2 = remove_from_hash_table(table, "test");
    // p_hashelem_t elem1 = remove_from_hash_table(table, "hello world");
    // print_hash_table(table);
    // free_hash_element(elem1, freedata);
    // free_hash_element(elem2, freedata);

    // p_tok_t token = split_str_into_tokens("hello,world", ',', 0);
    // print_p_toks_st(token);
    // free_p_toks_st(&token);
    // emulate(program6, sizeof(program6));
    // assemble("single.txt", 0);
    // emulate(program1, sizeof(program1));
    // printf("_______________________\n");
    // emulate(program2, sizeof(program2));
    // printf("_______________________\n");
    // emulate(program3, sizeof(program3));
    // printf("_______________________\n");
    // emulate(program4, sizeof(program4)/sizeof(*program4));
    // emulate(program5, sizeof(program5));
    // printf("______________________\n");
}

void test_asm()
{
	const char *dir = "multiply.txt";
    char *data = NULL;
	printf("\n");
}



void test_module()
{
	const char *path_riscv = "/Users/heathfortin/programing/myemu/myemu_new/build/src/device_libs/riscv/libriscv_isa.dylib";
	const char *path_basic_ram = "/Users/heathfortin/programing/myemu/myemu_new/build/src/device_libs/basic_ram/libbasic_ram_device.dylib";

	
	p_module_t riscv_module = open_module(path_riscv);
	p_module_t ram_module = open_module(path_basic_ram);

	//module assert
	
	assert(ram_module != NULL && "in this test module must not return null"); 
	assert(riscv_module != NULL && "in this test module must not return null"); 
	//get desc
	p_devdesc_t desc_riscv = get_device_from_module(riscv_module);	
	p_devdesc_t desc_ram = get_device_from_module(ram_module);	
	fprintf(stderr, "%s\n", strerror(errno));
	//description assert
	assert(desc_riscv != NULL && "description from a module must not return null");
	assert(desc_ram != NULL && "description from a module must not return null");

	printf("made it to the end\n");
	printf("%s\n",desc_riscv->lib_name);
	printf("%s\n",desc_ram->lib_name);

}

#include "tokenizer.h"
void test_pair_split(void)
{
	

}


void free_data(void *data)
{
	free(data);
}

#include "linked_list.h"

void test_linkedlist()
{
	p_linkedlist_t ll = new_linked_list_ptr(free_data);
	int *data = malloc(sizeof(int) * 10);
	for(int i = 0; i < 10; ++i)
	{
		data[i] = (int)i;
		printf("data[i] = %d\n", data[i]); 
	}
	insert_linked_list(ll, 0, data); 
	void *ptr = index_linked_list(ll, 0);
	for(int k = 0; k < 10; ++k)
	{
		printf("dup %d\n", ((int *)(ptr))[k]);
	}
}

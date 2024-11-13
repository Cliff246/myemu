#include "me_myemu.h"
#include "commons.h"
#include "lexer.h"
#include "me_test.h"

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
    const char *dir = "assembly_example/multiply.txt";
    char *data = NULL;
    int size = assemble(dir, &data);
}
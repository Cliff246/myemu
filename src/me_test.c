#include "inc/me_myemu.h"
#include "inc/me_test.h"


void test1()
{
    p_tok_t token = split_str_into_tokens("hello world game", ' ');
    print_p_toks_st(token);
    p_tok_t split = split_p_toks_st(token, 1);
    print_p_toks_st(split);
}

void test2()
{
    char buffer[1000];
    size_t size = getstr_with_cut_chars(buffer, 1000,"@a$top", "@$");
    DPRINTF("%s %d\n", buffer, size);
}

void testsuite()
{
    char *testprogram = (char *)program1;
    emulate(testprogram, sizeof(program1) / sizeof(*program1), CYCLES);
    p_tok_t token = split_str_into_tokens("hello world ghost ghost", ' ');
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
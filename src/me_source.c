#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "inc/me_myemu.h"
#include "inc/me_constants.h"



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

void testsuite()
{
    char *testprogram = (char *)program1;
    emulate(testprogram, sizeof(program1)/sizeof(*program1), CYCLES);
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
    //emulate(program6, sizeof(program6));
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



void readfile(char *str)
{
    const char* dir = str + 2;
    char *data = NULL;
    int size = assemble(dir, &data);
    emulate(data, size, CYCLES);
}

void outfile(char *out)
{

}

void testfile(char *str)
{
    const char *dir = "assembly_example/multiply.txt";
    char *data = NULL;
    int size =assemble(dir, &data);
    emulate(data, size, CYCLES);
}


typedef void (*argument_handler_t)(char *);

const argument_handler_t handler[3] = {
    readfile, outfile, testfile       
};

const char *arguments[] = {
    "-R",
    "-O",
    "-T",
};


int main(int argc, char *argv[])
{

    if(argc == 0 || argc == 1)
    {
        printf("NO arguments provided\n");
    }
    else
    {
            
        const int numargs = argc;
        for(int i = 1; i < numargs; i++)
        {
            const int strsize = strlen(argv[i]); 
            if(strsize < 2)
            {
                printf("argument not understood %s\n", argv[i]);
            }
            char firstbytes[3] = {argv[i][0], argv[i][1], 0};
            
            for(int options = 0; options < sizeof(arguments)/sizeof(arguments[0]); options++)
            {
                
                if(cmpstrings(firstbytes, arguments[options]))
                {
                    handler[options](argv[i]);
                }  

            }
        
        }    
                
        
        
    }

       
}

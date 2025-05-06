#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "commons.h"

#include "test.h"


#define TEST()

int start_tests()
{
    const int  tests_count = 0;
    printf("%d\n", tests_count);
    int success[tests_count];
    

    
    int retvalue = 0;
    int (* func[tests_count])();
    for(int i = 0; i < tests_count; i++)
    {
        int val = func[i]();
        success[i] = val; 
        retvalue |= val;
    } 
    
    return retvalue;
}




int main()
{
    start_tests();
	test1();
	printf("test");
	test_asm();
}

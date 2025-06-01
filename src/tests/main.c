#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "commons.h"

#include "tests_manager.h"
#include "test.h"






int main(int argc, char *argv[])
{

	collect_tests();
	if(argc == 2)
	{
		int data = atoi(argv[1]);
		int result = tests_registry[data].func();	
		printf("result: %d\n", result);
		return 0;
	}
	start_tests();
	display_tests();
}

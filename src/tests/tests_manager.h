#ifndef __TESTS_COMMONS__
#define __TESTS_COMMONS__

#include "commons.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <limits.h>

#define SKIP true
#define NOSKIP false

typedef int (*p_test_fn_t)(void);
typedef void (*p_test_collector_fn_t)(void);




typedef enum 
{
	TEST_OKAY,
	TEST_FAIL,
	TEST_SKIP,
}test_result_t; 

typedef struct test
{
	const char *name;
	const char *file;
	size_t line;
	p_test_fn_t func;
	test_result_t result;
	int result_value;
	int group;
	int index;
}test_t;


#ifndef __WIN32__
//unix shit


static_assert(sizeof(test_t) <= PIPE_BUF, "test_t is too big for a safe write");

typedef struct test_worker
{
	pid_t worker_pid;
	test_t *test_current;
	size_t start, end;
	int pipeline[2];	
}test_worker_t;

#else
//windows shit
typedef struct test_worker
{
	size_t start, end;

	//windows specific implementation of test_worker
	
}test_worker_t;
#endif


#define MAX_PROCESSES 8

#define MAX_TESTS 1024
#define MAX_COLLECTORS 128

//the registry of tests
extern test_t tests_registry[MAX_TESTS];
extern bool tests_registry_toskip[MAX_TESTS];
extern size_t tests_count;

//the collectors of tests
extern p_test_collector_fn_t tests_collectors[MAX_COLLECTORS];
extern size_t collectors_count;

int fork_tests(test_worker_t *worker);
void collect_tests(void);
void start_tests(void);
void display_tests(void);
void setup_test(void);
void add_test_collector(p_test_collector_fn_t fn);
void parent_watcher(test_worker_t *workers, int count);

#define TEST_GROUP(name) int name (void)
#define TEST(name) TEST_GROUP(name)
//yes ugh.... yes I know
#if !defined( __WIN32__ )
	#define TEST_COLLECTOR(fn) 												\
		static void fn(void);												\
		__attribute__((constructor)) static void auto_register_ ## fn(void)	\
		{																	\
			add_test_collector(fn);											\
		}																	\
		static void fn(void)												
#else
	#define TEST_COLLECTOR(fn) 												\
		static void fn(void)												
	#warning "win32 not supported yet"
#endif



#define REGISTER_TEST(fn, skip)                     			    \
																	\
    do																\
	{                                          						\
        if (tests_count < MAX_TESTS) 								\
		{            												\
            tests_registry[tests_count] = (test_t)					\
			{														\
                .name = #fn,                      					\
				.file = strdup(__FILE__),							\
				.line = __LINE__,									\
                .func = fn,                       					\
                .result = TEST_FAIL,               					\
				.index = tests_count								\
            }; 														\
			if(skip == true) 	 									\
				tests_registry_toskip[tests_count] = true;			\
			tests_count++;											\
        }															\
	   	else														\
	   	{                                  							\
																	\
            fprintf(stderr, "Too many tests!\n"); 					\
            exit(1);                              					\
        }                                        				 	\
    } 																\
	while (0)														\
	




#endif

#include "tests_manager.h"
#include "shared_utils_tests.h"

test_t tests_registry[MAX_TESTS];
size_t tests_count = 0;

test_t tests_registry[MAX_TESTS];
bool tests_registry_toskip[MAX_TESTS];
size_t tests_count;

p_test_collector_fn_t tests_collectors[MAX_COLLECTORS];
size_t collectors_count;


#ifndef __WIN32__
void forked_runner(test_worker_t *worker)
{
	signed long long size = (signed long long)worker->end - (signed long long)worker->start;
	if(size < 0)
	{
		exit(EXIT_FAILURE);
	}
	for(size_t i = worker->start, count = 0; i < worker->end; ++i, ++count)
	{
		worker->test_current = &tests_registry[i];
		worker->test_current->group = worker->worker_pid;
		if(tests_registry_toskip[i] == false)
		{
			int result = worker->test_current->func(); 
			if(result == 0)
			{
				worker->test_current->result = TEST_OKAY;	
			}	
			else
			{
				worker->test_current->result = TEST_FAIL;
			}
			worker->test_current->result_value = result;
		}
		else
		{
			worker->test_current->result = TEST_SKIP;
		}
		write(worker->pipeline[1], &worker->test_current, sizeof(test_t));	
	}




}
#endif 

void add_test_collector(p_test_collector_fn_t fn)
{
	if(collectors_count >= MAX_COLLECTORS)
	{
		perror("to many collectors");
		exit(EXIT_FAILURE);
	}
	tests_collectors[collectors_count++] = fn;
}

#if !defined(__WIN32__)
int fork_tests(test_worker_t *worker)
{
	pipe(worker->pipeline);
	pid_t pid = fork();
	
	if(pid == 0)
	{
		close(worker->pipeline[0]);	
		//child
		forked_runner(worker);
		close(worker->pipeline[1]);
		exit(EXIT_SUCCESS);	
	}
	else
	{
		close(worker->pipeline[1]);
		return pid;		
	}
}
#else
#warning "test suite not written in windows get to that..."
#endif 


void collect_tests(void)

{
	for(size_t i = 0; i < collectors_count; ++i)
	{
		//collect tests
		tests_collectors[i]();
	}
}

void start_tests()
{
		
	const unsigned max_proccesses = MAX_PROCESSES; 
	int current_proccesses = 0;
	int desired_proccesses = 0;
	size_t proc_range[max_proccesses] = {0};
	
	int leftover = tests_count % max_proccesses;
	int range = 0;
	if(tests_count == 0)
	{
		fprintf(stdout,"no tests lol\n");
		return;
	}
	if(tests_count < max_proccesses)
	{
		desired_proccesses = tests_count;
		range = 1;
		leftover = 0;
	}
	else
	{
		desired_proccesses = max_proccesses;
		range = tests_count / max_proccesses;	
	}

	test_worker_t workers[MAX_PROCESSES] = {0};	
	for(int proc_i = 0; proc_i < desired_proccesses; ++proc_i)
	{
		proc_range[proc_i] = range;
		if(proc_i == desired_proccesses - 1)
		{
			
			proc_range[proc_i] += leftover;
		}
	}	
		
		
	for(size_t fork_i = 0, start = 0; fork_i < desired_proccesses; ++fork_i)
	{
		workers[fork_i].start = start;
		workers[fork_i].end = start + proc_range[fork_i];
		fork_tests(&workers[fork_i]);	
		start = workers[fork_i].end;
	}
	
	parent_watcher(workers, desired_proccesses);
		
	for(size_t worker_i = 0, counter = 0; worker_i < desired_proccesses; ++worker_i)
	{
		test_t dest;
		while(read(workers[worker_i].pipeline[0], (void *)&dest, sizeof(test_t)) > 0)
		{

			int index = dest.index;

			tests_registry[index].group = dest.group;
			tests_registry[index].result = dest.result;
			tests_registry[index].result_value = dest.result_value;	
			
			counter++;	
		}
	}
		

}


void parent_watcher(test_worker_t *workers, int count)
{
	bool proccessing = true;
	int finished = 0;
	while(proccessing)
	{
		
		for (int i = 0; i < count; ++i)
   		{
			int status;
			pid_t pid = waitpid(workers[i].worker_pid, &status, 0);
			if(pid == 0)
				continue;
			else if(pid == -1)
			{
				finished++;				
				//some big error
			}
			else
			{
				if(WIFEXITED(status))
				{
					
					//restart
				}
				else if(WIFSIGNALED(status))
				{
					//killed
				}
				else
				{
					//noraml?
				}	
			}
			
		}		
		if(finished == count)
		{
			proccessing = false;
		}
	}


}



void display_tests(void)
{
	for(int i = 0; i < tests_count; ++i)
	{
		char *display = NULL;
		switch(tests_registry[i].result)
		{
			case TEST_OKAY:
				display = "PASSED";
				break;
			case TEST_FAIL:
				display = "FAILED";
				break;
			case TEST_SKIP:
				display = "SKIPPED";
				break;
		}
		printf("tests: %d\n\ttest:%s\n\t%s:%zu\n", i, display, tests_registry[i].file, tests_registry[i].line);
	}

}



#include "../headers/repl.h"

#include "tokenizer.h"

char *get_line(const char *start)
{
	size_t allocd = 100;
	size_t count = 0;
	char *buffer = malloc(sizeof(char) * allocd);
	if(buffer == NULL)
	{
   		errno = ENOMEM;
		return NULL;
	}
	memset(buffer, '\0', allocd); 
	char current_char;
	fprintf(stdout, "%s", start);
	do
	{		
		current_char	= getc(stdin);
		buffer[count++] = current_char;
		if(count >= allocd)
		{
			size_t temp_new_size = allocd * 2 + 1;
			void *temp = realloc(buffer, temp_new_size);
			if(temp)
			{
				memset((char *)temp + allocd , '\0', allocd);
			   	buffer = temp;
				allocd = temp_new_size;	
			}
			else
			{
				free(buffer);
				errno = ENOMEM;
				return NULL;
			}
		}	
	}	
	while(current_char != '\0' && current_char != '\n');
	if(current_char == '\n')
	{
		buffer[count] = '\0';
	}	
	return buffer;
}



void runtime(void)
{
	

	bool quit = false;
	const char *inp_str = ": ";	
	const char *sep = " :\"?\\/";
	while(quit != true)
	{
		
		char *line = get_line(inp_str);
		p_tok_t tokens = split_str_into_tokens(line, sep);
		free(line);	
		print_p_toks_st(tokens);
		free_p_toks_st(tokens);
	}

}

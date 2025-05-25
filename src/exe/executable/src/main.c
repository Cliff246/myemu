#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


char *get_line()
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


int main(int argc, char *argv[])
{
	char *line = get_line();
	printf("line %s\n", line);
}

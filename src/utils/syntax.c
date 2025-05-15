#include "commons.h"
#include "syntax.h"
#include "inst.h"

int get_id()
{
	static int count = 0;
	return ++count;
}


p_syntax_t create_syntax_t(char *base, char *keyword)
{
	if(base && keyword)
	{
		p_syntax_t new = (p_syntax_t)calloc(1, sizeof(syntax_t));
		if(!new)
		{
			errno = ENOMEM;
			return NULL;
		}
		new->keyword = strdup(keyword);
		new->base = strdup(base);
		
		if(new->keyword == NULL || new->base == NULL)
		{
			free(new);
			errno = ENOMEM;
			return NULL;
		}


		new->id = get_id();
		return new;
	}
	errno = EINVAL;
	return NULL;
}

void add_syntax_argument_types(p_syntax_t syn, char **argtypes, size_t size)
{
	if(!syn)
	{
		errno = EINVAL;
		return;
	}
	char **memalloc	= (char **)calloc(size, sizeof(char *));
	if(memalloc == NULL)	
	{
		errno = ENOMEM;
		return;
	}
	
	char *temp_holder[size];
	size_t i = 0;
	for(; i < size; ++i)
	{
		if(argtypes[i] == NULL)
		{	
			goto fail;
		}
		else
		{
			char *copy =  strdup(argtypes[i]);


			if(copy == NULL)
			{

				goto fail;
			}	
			temp_holder[i] = copy;
		}
	}
	if(memalloc != memcpy(memalloc, temp_holder, sizeof(char *) * size))
	{
		goto fail;
	}
	syn->arg_types = memalloc;	
	syn->arg_count = size;
	return;
		

	fail:
	for(size_t i_fail = 0; i_fail < i; ++i_fail)
	{
		free(temp_holder[i_fail]);
	}
	free(memalloc);
	errno = EINVAL;
	return; 
}

void add_syntax_lables(p_syntax_t syn, char **labels, size_t size)
{
	if(!syn)
	{
		errno = EINVAL;
		return;
	}
	char **memalloc	= (char **)calloc(size, sizeof(char *));
	if(memalloc == NULL)	
	{
		errno = ENOMEM;
		return;
	}
	
	char *temp_holder[size];
	size_t i = 0;
	for(; i < size; ++i)
	{
		if(labels[i] == NULL)
		{	
			goto fail;
		}
		else
		{
			char *copy =  strdup(labels[i]);


			if(copy == NULL)
			{

				goto fail;
			}	
			temp_holder[i] = copy;
		}
	}
	if(memalloc != memcpy(memalloc, temp_holder, sizeof(char *) * size))
	{
		goto fail;
	}
	syn->labels = memalloc;	
	syn->labels_count = size;
	return;
		

	fail:
	for(size_t i_fail = 0; i_fail < i; ++i_fail)
	{
		free(temp_holder[i_fail]);
	}
	free(memalloc);
	errno = EINVAL;
	return;
}

void add_syntax_base_binary(p_syntax_t syn, char **base_binary, size_t size)
{
	if(!syn)
	{
		return;
	}
	char **memalloc	= (char **)calloc(size, sizeof(char *));
	if(memalloc == NULL)	
	{
		errno = ENOMEM;
		return;
	}
	
	char *temp_holder[size];
	size_t i = 0;
	for(; i < size; ++i)
	{
		if(base_binary[i] == NULL)
		{	
			goto fail;
		}
		else
		{
			char *copy =  strdup(base_binary[i]);


			if(copy == NULL)
			{

				goto fail;
			}	
			temp_holder[i] = copy;
		}
	}
	if(memalloc != memcpy(memalloc, temp_holder, sizeof(char *) * size))
	{
		goto fail;
	}
	syn->base_binary= memalloc;	
	syn->base_binary_count = size;
	return;
		

	fail:
	for(size_t i_fail = 0; i_fail < i; ++i_fail)
	{
		free(temp_holder[i_fail]);
	}
	free(memalloc);
	errno = EINVAL;
	return;
}

void add_syntax_offsets(p_syntax_t syn, size_t *offsets, size_t size)
{
	if(!syn)
	{
		return;
	}
	size_t *memalloc	= (size_t *)calloc(size, sizeof(size_t));
	if(memalloc == NULL)	
	{
		errno = ENOMEM;
		return;
	}
	
	size_t i = 0;
	for(; i < size; ++i)
	{
		memalloc[i]=offsets[i];
		
	}
	syn->offset = memalloc;	
	syn->offset_counts = size;
	return;
}


void add_syntax_modifiers(p_syntax_t syn, char **modifiers, size_t size)

{
	if(!syn)
	{
		errno = EINVAL;
		return;
	}
	char **memalloc	= (char **)calloc(size, sizeof(char *));
	if(memalloc == NULL)	
	{
		errno = ENOMEM;
		return;
	}
	
	char *temp_holder[size];
	size_t i = 0;
	for(; i < size; ++i)
	{
		if(modifiers[i] == NULL)
		{	
			goto fail;
		}
		else
		{
			char *copy =  strdup(modifiers[i]);


			if(copy == NULL)
			{

				goto fail;
			}	
			temp_holder[i] = copy;
		}
	}
	if(memalloc != memcpy(memalloc, temp_holder, sizeof(char *) * size))
	{
		goto fail;
	}
	syn->modifiers = memalloc;	
	syn->modifiers_count = size;
	return;
		

	fail:
	for(size_t i_fail = 0; i_fail < i; ++i_fail)
	{
		free(temp_holder[i_fail]);
	}
	free(memalloc);
	errno = EINVAL;
	return;	
}

int64_t add_to_syntax_map(p_hashtable_t table, p_syntax_t syn)
{
	


	return	addto_hash_table(table, syn->keyword, (void *)syn);
}



void free_syntax_ptr(void *syntax)
{
	if(syntax != NULL)
		return;
	p_syntax_t syn = (p_syntax_t)syntax;

	free(syn->offset);
	for (size_t i_labels = 0; i_labels < syn->labels_count; ++i_labels)
	{
		free(syn->labels[i_labels]);
	}	
	free(syn->labels);

	for (size_t i_args = 0; i_args < syn->arg_count; ++i_args)
	{
		free(syn->arg_types[i_args]);
	}
	free(syn->arg_types);
	for (size_t i_base_bin = 0; i_base_bin < syn->base_binary_count; ++i_base_bin)
	{

		free(syn->base_binary[i_base_bin]);
	}
	free(syn->base_binary);
	for (size_t i_mods= 0; i_mods< syn->modifiers_count; ++i_mods)
	{

		free(syn->modifiers[i_mods]);
	}
	free(syn->modifiers);
	free(syn->keyword);
	free(syn->base);
	free(syn);
}


bool validate_asminst_is_valid(p_asminst_t asminst, p_syntax_t syntax )
{
	return false;
}

char *convert_asminst_to_bytes(p_asminst_t asminst, p_syntax_t syntax )
{
	
	if(asminst == NULL ||  syntax == NULL)
	{
		return NULL;
	}


	

	return NULL;	
}

 

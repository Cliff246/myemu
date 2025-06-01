#include "../headers/tokenizer.h"
#include "commons.h"
#include <stdbool.h>
#include <string.h>
#include "linked_list.h"

void print_p_toks_st(p_tok_t token)
{

    printf("--- p_toks %p ---\n tokenary_len = %lu\n", (void *)&token, token->nstr);

    if (token->nstr == 0)
        return;

    for (int iter = 0; iter < token->nstr; iter++)
    {
        for (char *c = token->p_sz_toks[iter]; *c != 0; c++)
        {
            if (*c == '\n')
                *c = 0;
        }

        printf(" token = '%s' col = %lu\n", token->p_sz_toks[iter], token->p_u_col[iter]);
    }
}

void print_p_toks_string(p_tok_t token, bool offset)
{
    // fprintf(stderr,"LINE %s:%d\n", __FILE__, __LINE__);

    if (token == NULL)
    {
        fprintf(stderr,"failed token%p\n", token);
        return;
    }
    else if (token->nstr == 0)
    {
        // fprintf(stderr,"LINE %s:%d\n", __FILE__, __LINE__);

        return;
    }
    else
    {
        // fprintf(stderr,"LINE %s:%d\n", __FILE__, __LINE__);

        //  fprintf(stderr,"%d\n", token->nstr);

        size_t last = 0;
        for (int iter = 0; iter < token->nstr; iter++)
        {
            if (offset)
            {
                for (size_t col = last; col < token->p_u_col[iter]; col++)
                {
                    printf(" ");
                }
            }
            last = token->p_u_col[iter];

            for (char *c = token->p_sz_toks[iter]; *c != 0; c++)
            {
                if (*c == '\n')
                    *c = 0;
                printf("%c", *c);
                last++;
            }
        }
    }
    printf("\n");
}

char *alloc_p_toks_string(p_tok_t ref, bool offset)
{

    size_t size = 1;

    for (size_t i = 0; i < ref->nstr; i++)
    {
        size += ref->p_u_col[i];
    }
    size += strlen(ref->p_sz_toks[ref->nstr - 1]);
    char *buffer = calloc(size, sizeof(char));
    if (size == 1)
    {
        return buffer;
    }
    size_t last = 0;
    size_t bufferiter = 0;
    for (int iter = 0; iter < ref->nstr; iter++)
    {
        if (offset)
        {
            for (size_t col = last; col < ref->p_u_col[iter]; col++)
            {
                buffer[bufferiter++] = ' ';
            }
        }
        last = ref->p_u_col[iter];

        for (char *c = ref->p_sz_toks[iter]; *c != 0; c++)
        {
            if (*c == '\n')
                *c = 0;
            buffer[bufferiter++] = *c;
            last++;
        }
    }
    return buffer;
}

p_tok_t malloc_p_toks_st()
{
    p_tok_t tok = (p_tok_t)calloc(1, sizeof(tok_t));
    if (!tok)
    {
        fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    tok->nstr = 0;
    tok->p_sz_toks = NULL;
    tok->p_u_col = NULL;
    return tok;
}

void free_p_toks_st(p_tok_t token)
{
    const p_tok_t tok = token;

    if (token == NULL)
    {
        fprintf(stderr,"wtf are you doing not even deleting known data. we need to set your toks data to 0 %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    if (tok != NULL)
    {
        for (size_t i = 0; i < (tok)->nstr; i++)
        {
            if (tok->p_sz_toks[i])
                free(tok->p_sz_toks[i]);
        }
        free(tok->p_sz_toks);
        free(tok);
        // fprintf(stderr,"freed token %p\n", token);
        //  I AM GOING TO TRUST THIS IS FREED CAUSE (IDOIT)
    }
    else
    {
        free(tok);
        // fprintf(stderr,"token ptr not defined, freed anyways %s:%d\n", __FILE__, __LINE__);
        //  fuck it free it anyways
    }
}

char **copy_all_strary(char **src, size_t src_size)
{
    if (!src)
    {
        return NULL;
    }
    char **copy = (char **)calloc(src_size, sizeof(char *));
    if (!copy)
    {
        fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t iter = 0; iter < src_size; iter++)
    {
        char *refstr = src[iter];
        const size_t reflen = strlen(refstr) + 1;
        char *cpystr = (char *)calloc(reflen, sizeof(char));

        if (!cpystr)
        {
            fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }

        strcpy(cpystr, refstr);
        copy[iter] = cpystr;
    }
    return copy;
}

p_tok_t cut_p_toks_st(p_tok_t ref, size_t nstart, size_t nstop)
{
    if (!ref)
    {
        fprintf(stderr,"failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    const int64_t delta_len = nstop - nstart;
    if (ref->nstr < delta_len || ref->nstr == 0 || delta_len < 0)
    {
        fprintf(stderr,"failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    p_tok_t new = malloc_p_toks_st();
    const size_t new_len = delta_len;

    size_t *column_copy = (size_t *)calloc(new_len, sizeof(size_t));
    if (!column_copy)
    {
        fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    memcpy(column_copy, ref->p_u_col + nstart, new_len * sizeof(size_t));
    char **str_copy = copy_all_strary(ref->p_sz_toks + nstart, nstop);

    new->p_u_col = column_copy;
    new->p_sz_toks = str_copy;
    new->nstr = new_len;
    return new;
}

p_tok_t split_p_toks_st(p_tok_t ref, size_t nsplit)
{
    if (!ref)
    {
        fprintf(stderr,"failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    if (ref->nstr < nsplit || ref->nstr == 0)
    {
        fprintf(stderr,"failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    p_tok_t new = malloc_p_toks_st();
    const size_t new_len = ref->nstr - nsplit;

    size_t *column_copy = (size_t *)calloc(new_len, sizeof(size_t));
    if (!column_copy)
    {
        fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    memcpy(column_copy, ref->p_u_col + nsplit, new_len * sizeof(size_t));
    char **str_copy = copy_all_strary(ref->p_sz_toks + nsplit, new_len);

    new->p_u_col = column_copy;
    new->p_sz_toks = str_copy;
    new->nstr = new_len;

    return new;
}

p_tok_t copy_p_toks_st(p_tok_t ref)
{
    if (!ref)
    {
        DPRINT("failed\n");
        exit(1);
    }
    const size_t len = ref->nstr;
    p_tok_t new = malloc_p_toks_st();
    if (len == 0)
    {
        // DPRINT("len == 0\n");
        new->p_u_col = NULL;
        new->p_sz_toks = NULL;
        new->nstr = 0;
        return new;
    }
    else
    {
        size_t *column_copy = (size_t *)calloc(len, sizeof(size_t));

        if (!column_copy)
        {
            fprintf(stderr,"MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }
        memcpy(column_copy, ref->p_u_col, len * sizeof(size_t));
        char **str_copy = copy_all_strary(ref->p_sz_toks, len);

        new->p_u_col = column_copy;
        new->p_sz_toks = str_copy;
        new->nstr = len;
        // DPRINT("new\n");
        // print_p_toks_st(new);
        // print_p_toks_string(new, true);
        return new;
    }
}

void update_p_toks_st(p_tok_t ref_tok, size_t length)
{
    p_tok_t ptok = ref_tok;
    if (length == 0)
    {
        ptok->p_u_col = NULL;
        ptok->p_sz_toks = NULL;
        ptok->nstr = length;
        return;
    }
    if(ref_tok->nstr == 0)
    {
        void *tmp1 = calloc(length, sizeof(size_t));
        void  *tmp2 = calloc(length, sizeof(char *));
        if (!tmp1 || !tmp2)
        {
            exit(1);
        }
        ptok->p_u_col = (size_t *)tmp1;
        ptok->p_sz_toks = (char **)tmp2;
        ptok->nstr = length;
        for (size_t init_elem = 0; init_elem < ptok->nstr; init_elem++)
        {
            if (ptok->p_sz_toks[init_elem] != NULL)
                continue;
            else
            {
                char *str = (char *)calloc(1, sizeof(char));
                if (!str)
                    exit(1);
                ptok->p_sz_toks[init_elem] = str;
            }
        }
    }
    else
    {
        const size_t orignlen = ptok->nstr;
        ptok->p_u_col = REALLOC_SAFE(ptok->p_u_col, length * sizeof(size_t));
        ptok->p_sz_toks = REALLOC_SAFE(ptok->p_sz_toks, length * sizeof(char *));
        ptok->nstr = length;


        for (size_t init_elem = orignlen; init_elem < ptok->nstr; init_elem++)
        {
            //if (ptok->p_sz_toks[init_elem] != NULL)
            //    free(ptok->p_sz_toks[init_elem]);
            char *str = (char *)calloc(1, sizeof(char));
            if (!str)
                exit(1);
            ptok->p_sz_toks[init_elem] = str;
            ptok->p_u_col[init_elem] = 0;
        }

    }
    return;
}

//cut substring
p_tok_t cut_substr_p_tok_t(p_tok_t ref, const char *substr)
{
    size_t newlen = 0;
    bool *toremove_ary = (bool *)calloc(ref->nstr, sizeof(bool));

    for (size_t i = 0; i < ref->nstr; i++)
    {
        if (!cmpstrings(ref->p_sz_toks[i], substr))
        {
            //fprintf(stderr,"toks |%s| substr |%s|\n", ref->p_sz_toks[i], substr);
            newlen++;
            toremove_ary[i] = false;
        }
        else
        {
            toremove_ary[i] = true;
        }
    }
    p_tok_t new = malloc_p_toks_st();

    update_p_toks_st(new, newlen);
    size_t new_iter = 0;
    for (size_t n = 0; n < ref->nstr; n++)
    {
        if (!toremove_ary[n])
        {
            const size_t templen = strlen(ref->p_sz_toks[n]);
            char *temp = REALLOC_SAFE(new->p_sz_toks[new_iter], templen + 1);
            memset(temp, 0, templen + 1);
            strcpy(temp, ref->p_sz_toks[n]);
            new->p_sz_toks[new_iter] = temp;
            new->p_u_col[new_iter] = ref->p_u_col[n];
            new_iter++;
        }
    }
    free(toremove_ary);
    return new;
}

p_tok_t split_str_into_tokens(char *inp, const char *sep)
{

    if (inp == NULL)
        return NULL;

    p_tok_t p_token_st = malloc_p_toks_st();
    if (cmpstrings(inp, ""))
    {

        return p_token_st;
    }
    size_t nsep = 0, ntok = 1;

    bool prev_is_sep = char_exist(inp[0], (char *)sep);
    char last_sep = inp[0];

    for (char *cur = inp + 1; *cur != 0; cur++)
    {
        char this_sep = *cur;
        bool is_sep = char_exist(this_sep, (char *)sep);


        if (is_sep && this_sep != last_sep)
        {
            nsep += 1;
        }
        if(is_sep == false && prev_is_sep == true)
        {
            nsep += 1;
        }
        last_sep = this_sep;

        prev_is_sep = is_sep;
        //fprintf(stderr,"%d %d %d |%c| |%c|\n", is_sep, prev_is_sep, nsep, last_sep, this_sep);
    }
    if(nsep == 0)
    {
       // fprintf(stderr,"%d\n", nsep);
        ntok = 1;
    }
    else
    {
        ntok += nsep;
    }
    update_p_toks_st(p_token_st, ntok);

    size_t prev = 0, current = 0, itr = 0;
    size_t *colary = (size_t *)malloc(sizeof(size_t) * ntok);
    if (!colary)
        exit(1);
    last_sep = inp[0];
    prev_is_sep = char_exist(inp[0], (char *)sep);
    //fprintf(stderr,"%s\n", inp);
   // fprintf(stderr,"%d\n", nsep);

    for (char *n = inp ; *n != 0; n++, current++)
    {
        char this_sep = *n;
        bool is_sep = char_exist(this_sep, (char *) sep);
        //fprintf(stderr,"|%c| %d %d\n", *n, is_sep, prev_is_sep);
        // fprintf(stderr,"%s\n", n);

        //fprintf(stderr,"%d %d %d |%c| |%c|\n", is_sep, prev_is_sep, nsep, last_sep, this_sep);
        if (is_sep && prev_is_sep && last_sep != this_sep)
        {
            // fprintf(stderr,"set %d\n",itr);

            colary[itr] = prev;
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], 2);

            p_token_st->p_sz_toks[itr][0] = last_sep;
            p_token_st->p_sz_toks[itr][1] = 0;
            prev = current;

            itr++;
        }
        if (!is_sep && prev_is_sep)
        {
            colary[itr] = prev;
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], 2 * sizeof(char));
            p_token_st->p_sz_toks[itr][0] = last_sep;
            p_token_st->p_sz_toks[itr][1] = 0;
            // fprintf(stderr,"%s\n", p_token_st->p_sz_toks[itr]);
            // fprintf(stderr,"set %d\n", itr);
            prev = current;

            itr++;
        }
        if (is_sep && !prev_is_sep)
        {
            colary[itr] = prev;
            // fprintf(stderr,"current - prev + 1 %d\n", current - prev + 1);
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current - prev + 1);
            memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);
            // fprintf(stderr,"%s\n", p_token_st->p_sz_toks[itr]);
            p_token_st->p_sz_toks[itr][current - prev] = 0;
            // fprintf(stderr,"set %d\n", itr);
            prev = current;

            itr++;

        }


        prev_is_sep = is_sep;
        last_sep = this_sep;
    }
    if (char_exist(last_sep,(char *) sep))
    {
        //fprintf(stderr,"%c\n", last_sep);
        colary[itr] = prev;
        (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], 2 * sizeof(char));
        p_token_st->p_sz_toks[itr][0] = last_sep;
        p_token_st->p_sz_toks[itr][1] = 0;
        // fprintf(stderr,"%s\n", p_token_st->p_sz_toks[itr]);
        // fprintf(stderr,"set %d\n", itr);
    }
    else
    {
    //fprintf(stderr,"%d %d\n", ntok, itr);
        size_t remaining = strlen(inp + prev);
        colary[itr] = prev;
    //fprintf(stderr,"set %d %s\n", itr, inp + prev);
        p_token_st->p_sz_toks[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], remaining  + 1);
        memset(p_token_st->p_sz_toks[itr], 0, remaining + 1);
        memcpy(p_token_st->p_sz_toks[itr], inp + prev, remaining );
    //fprintf(stderr,"|%s|\n", inp + prev);
    }
    memcpy(p_token_st->p_u_col, colary, sizeof(size_t) * (ntok));
    //print_p_toks_st(p_token_st);

    free(colary);

    return p_token_st;
}


p_tok_t merge_p_toks(p_tok_t tok_a, p_tok_t tok_b)
{
	
	const size_t needed_size = tok_a->nstr + tok_b->nstr;
	
	p_tok_t combine = malloc_p_toks_st();
	update_p_toks_st(combine, needed_size);
	
	for(size_t i = 0; i < tok_a->nstr; ++i)
	{
		combine->p_sz_toks[i] = strdup(tok_a->p_sz_toks[i]);
		const size_t a_column = tok_a->p_u_col[i];
		combine->p_u_col[i]	= a_column;

	}
	const size_t last = tok_a->nstr - 1;
	const size_t last_size = strlen(tok_a->p_sz_toks[last]) + tok_a->p_u_col[last];
	const size_t start = tok_b->nstr;	
	for(size_t k = 0; k < tok_b->nstr; ++k)
	{
		combine->p_sz_toks[start + k] = strdup(tok_b->p_sz_toks[k]);

		combine->p_u_col[k]	= last_size + tok_b->p_u_col[k];
	
	}
	combine->nstr = needed_size;
	return combine;
}

int get_tok_i_column(p_tok_t ref, int index)
{
	if(ref->nstr > index)
	{
		if(index == 0)
		{
			return 0;
		}
		
		return ref->p_u_col[index - 1];

	}
	else
	{
		return -1;
	}	
}





static void free_delim_index_remover(void *data)
{
	return;
}

static int get_data_length_from_ll(void *data)
{
	return sizeof(int); 
}	

static int convert_to_data_stream_ll(void *data, char *buffer)
{
	//lol i know
	const int *stream_data	= (int *)data;
	memcpy(buffer, stream_data, sizeof(int));	
	return sizeof(int);
}

//order must be characters for open and closed. if not it will die quickly
int pair_toks_delims(p_tok_t ref, p_tok_t *dest, const char *delims)
{

	struct pair
	{
		int delim_index;	
		int start;
		int end;	
	};
	int error = 0;
	
	size_t delims_len = strlen(delims);
	if(delims_len % 2 == 1)
		return (error = 1);
	size_t groups = delims_len /2;
	p_tok_t destination = copy_p_toks_st(ref);	
	
	
	int allocated = 100;
	size_t pairs_size = 0;
	//array of delims
	struct pair *delim_pairs = (struct pair *)calloc(allocated, sizeof(struct pair));	
	//current pair
	int *pairs_current = (int *)calloc(groups, sizeof(int));
	//memset so everything is not valid
	memset(pairs_current, 0, groups * sizeof(int));
	//index
	p_linkedlist_t *pairs_index = (p_linkedlist_t *)calloc(groups, sizeof(p_linkedlist_t)); 	
	int *pairs_index_scope =  (int *)calloc(groups, sizeof(int));
	for(size_t lli = 0; lli < groups; ++lli)
	{
		pairs_index[lli] = new_linked_list_inline(free_delim_index_remover, convert_to_data_stream_ll, get_data_length_from_ll); 
		
	}
	const int size = 256;
	//do not fuck with this
	char pair_mask[size] = {0};
	bool pair_same[size] = {0};	
	int count = 1;
	for(size_t d = 0; d < delims_len; d+=2)
	{	
		unsigned char spot1 = delims[d], spot2 = delims[d + 1];
		pair_mask[(unsigned char)delims[d]] = count;
		pair_mask[(unsigned char)delims[d + 1]] = count++;
		bool same = (delims[d] == delims[d+1])? true : false;
		if(same)
		{

			pair_same[(unsigned char)delims[d]] = same;
		}


		printf(" %c %c %d %d  %d %d\n",delims[d], delims[d+1],pair_mask[spot1], pair_mask[spot2], pair_same[spot1], pair_same[spot2]);
	}	
	//fuck done
	int current_pair = 0;
	size_t *tok_len_cache = (size_t *)malloc(sizeof(size_t) * ref->nstr);
	size_t total_len = 0;
	for(size_t i = 0; i < ref->nstr; ++i)
	{
		size_t tok_psz_len = strlen(ref->p_sz_toks[i]);	
		total_len += tok_psz_len;
		tok_len_cache[i] = tok_psz_len;
		for(size_t psz_i = 0; psz_i < tok_psz_len; ++psz_i)
		{
			
			char current = ref->p_sz_toks[i][psz_i];
			char mask = pair_mask[current];
			int group_point = (int)mask / 2;	
			//in mask			
			if(mask > 0)
			{


				int scope = pairs_index_scope[group_point];	
				//double check if delim is in mask?
				bool outin = (current == delims[mask])? true: false;

				//column of char
				int column = get_tok_i_column(ref, i);					
				printf("outin %c %d\n",current, outin); 
				printf("pair same %d\n", pair_same[group_point]);
				if(pair_same[current] == true)
				{
					if(scope == 1)
					{
						int get_recent = *(int *)index_linked_list(pairs_index[group_point], 0);

						delim_pairs[get_recent].start = pairs_current[group_point];	
						delim_pairs[get_recent].end = column  + psz_i;
						delim_pairs[get_recent].delim_index = group_point;
						scope--;
					}
					else if(scope == 0)
					{

						int copy = current_pair;
						insert_linked_list(pairs_index[group_point], 0, &copy);
						pairs_current[group_point] = column + psz_i;
						scope++;
						current_pair++;

					}
					else
					{
						printf("the fuck\n");
					}
				}
				else
				{
					if(outin == true)
					{
						const int pos = column + psz_i;

						
						//is starter
						pairs_current[group_point] = pos;
						insert_linked_list(pairs_index[group_point], 0, &current_pair);
						
						delim_pairs[current_pair].start = pos;
						scope++;
						current_pair++;
					}
					else if(outin == false)
					{
						//is ender
						if(scope < 0)
						{
							printf("fucked\n");
							exit(1);

						}	
						int get_recent = *(int *)index_linked_list(pairs_index[group_point], scope - 1);
						printf("get recent%d\n", get_recent);
						if(scope == 0)
						{
							printf("not overwriting\n");
							delim_pairs[get_recent].start = pairs_current[group_point];

						}
						delim_pairs[get_recent].end = column  + psz_i;
						delim_pairs[get_recent].delim_index = group_point;
						scope--;	
					}		
					else
					{

					}
						
				}
				pairs_index_scope[group_point] = scope;
			}
		}
	}
	int cuts = 0;



	size_t token_scroll = 0;	
   	size_t sum = sum + tok_len_cache[token_scroll];
	size_t prev_sum = 0;
	size_t current_cache = tok_len_cache[token_scroll];
	for(int cut_i  = 0; cut_i < current_pair; ++cut_i)
	{
		
		int tok_zone = 0;
		int cut_start = delim_pairs[cut_i].start, cut_end = delim_pairs[cut_i].end;
		
		

		if(cut_start < sum + tok_len_cache[token_scroll])
		{
			if(cut_start - prev_sum >= tok_len_cache[token_scroll])
			{

			}
			else
			{

			}
		}
		//carry over
		if(cut_end > sum)
		{
			prev_sum = sum;
			
			if(token_scroll <= ref->nstr)
			{

				sum = sum + tok_len_cache[++token_scroll];
					
			}
			else
			{
				fprintf(stderr,"end went over limits\n");
			}
		}
	}



	for(int to_free_i = 0; to_free_i < groups; to_free_i++)
	{
		free_linked_list(pairs_index[to_free_i]);
	}
	free(pairs_current);
	free(pairs_index);
	free(pairs_index_scope);
	free(delim_pairs);
	free(tok_len_cache);
	return error;
}


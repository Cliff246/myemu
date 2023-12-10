#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "me_commons.h"

#define MAX_FILE_SIZE 1024
#define MAX_LINE_SIZE 1024



typedef char * string_t;
const string_t empty = "";

typedef struct tokens
{
    string_t *p_sz_toks;
    size_t *p_u_col;
    size_t nstr, line;
}tok_t, *p_tok_t;



void print_p_toks_st(p_tok_t token)
{

    DPRINTF("--- p_toks %p ---\n line_num =%d, tokenary_len = %d\n", &token, token->line, token->nstr);
    
    if(token->nstr == 0)
        return;
    
    for(int iter = 0; iter < token->nstr; iter++)
    {   
        for(char *c = token->p_sz_toks[iter]; *c != 0; c++)
        {
            if(*c == '\n')
                *c = 0;
        }

        DPRINTF(" token = '%s' col = %d\n", token->p_sz_toks[iter], token->p_u_col[iter]);
    }
}
p_tok_t malloc_p_toks_st()
{
    p_tok_t tok = (p_tok_t)calloc(1, sizeof(tok_t));
    if(!tok)
    {
        DPRINTF("big oops on calloc %s %d\n", __FILE__,__LINE__);
        exit(1);
    }
    tok->line = 0;
    tok->nstr = 0;
    tok->p_sz_toks = NULL;
    tok->p_u_col = NULL;
    return tok;
}

void update_p_toks_st(p_tok_t ref_tok, size_t length, size_t nline)
{
    p_tok_t ptok = ref_tok;
    if(length == 0)
    {
        ptok->p_u_col = NULL;
        ptok->p_sz_toks = NULL;
        ptok->nstr = length;
        ptok->line = nline;
        return;
    }
    void 
        *tmp1 = calloc(length, sizeof(size_t)),
        *tmp2 = calloc(length, sizeof(string_t));
    ptok->p_u_col = (size_t*)tmp1;
    ptok->p_sz_toks = (string_t*)tmp2;
    ptok->nstr = length;
    ptok->line = nline;
    for(size_t init_elem = 0; init_elem < ptok->nstr; init_elem++)
    {
        if(ptok->p_sz_toks[init_elem] != NULL)
            continue;
        else
        {
            string_t str = (string_t)calloc(1, sizeof(char));
            if(!str)
                exit(1);
            ptok->p_sz_toks[init_elem] = str;

        }
    }    
    return;
}


p_tok_t split_str_into_tokens(string_t inp, char sep, size_t line)
{
    if(inp == NULL)
        return NULL;
    p_tok_t p_token_st = malloc_p_toks_st();
    size_t inp_length = strnlen(inp, MAX_FILE_SIZE);
    size_t nsep = 1;

    if(inp_length == 0)
    {
        update_p_toks_st(p_token_st, 0, line);
        return p_token_st;    
    }
    else
    {
        for(char *cur = inp + 1, cprev = *inp; *cur != 0; cur++)
        {
            if(*cur == sep && cprev != sep)
            {
                printf("%c\n", *cur);
                nsep+=1;   
            }
            cprev = *cur;
        }
    }
    update_p_toks_st(p_token_st, nsep, line);

    size_t prev = 0, current = 1, itr = 0;
    size_t *colary = (size_t*)malloc(sizeof(size_t) * nsep);

    char pchar = inp[0];
    for(char *n = inp + 1; *n != 0; n++, current++)
    {
        if(*n == sep && pchar != sep)
        {
            colary[itr] = prev;
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current - prev + 1);
            memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);
            p_token_st->p_sz_toks[itr][current - prev + 1] = 0;
            itr++;
        }
        if(*n != sep && pchar == sep)
            prev = current;
        pchar = *n;
    }   
    colary[nsep - 1] = prev;
    p_token_st->p_sz_toks[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], (current + prev + 1) * sizeof(char));
   
    memcpy(p_token_st->p_u_col, colary, nsep * sizeof(size_t));
    memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);
    p_token_st->p_sz_toks[itr][current - prev] = 0;
    return p_token_st;
}

size_t get_tokens(p_tok_t **reftok,char *dir)
{
    FILE *pfile = fopen(dir, "r");
    if(pfile)
    {
        size_t fchars = 0;
        size_t numline = 0;
        p_tok_t *tokens = (p_tok_t*)calloc(numline, sizeof(p_tok_t));

        size_t len_history = 1000, i_history = 0, tokiter = 0;
        char *history =(char*)calloc(len_history + 1, sizeof(char));
        char c = fgetc(pfile);
        bool gogo = true;
        while(gogo)
        {   
            if(c == EOF)
                gogo = false;
            history[i_history++] = c;
            if(c == '\n' || c == EOF)
            {   
                trimr(history);
                p_tok_t tok = split_str_into_tokens(history, ' ', numline);
                tokens = (p_tok_t *)REALLOC_SAFE(tokens, sizeof(p_tok_t) * (numline + 1));
                tokens[tokiter++] = tok;
                print_p_toks_st(tok);
                i_history = 0;
                len_history = 1000;
                history = REALLOC_SAFE(history, len_history + 1);
                memset(history, 0, len_history + 1);
                numline++;
            }
            if(i_history == len_history - 1)
            {
                history = REALLOC_SAFE(history, len_history += 1001);
            }
            feof(pfile);
            fchars++;
            c = getc(pfile);
            
        }
        
        free(history);
        fclose(pfile);
        DPRINTF("closing %s %d\n", dir, numline);
        *reftok = tokens;
        return numline;
    }
    return 0;
}
void assemble(char *dir, size_t size)
{

    p_tok_t *tokens = NULL;
    size_t lines = get_tokens(&tokens,dir); 
    
    DPRINTF("%d\n", lines);
    size_t memorysize = 1;
    char *program = (char *)malloc(1);
    for(size_t iline = 0; iline < lines; iline++)
    {
        p_tok_t curline = tokens[iline];
        if(curline->nstr == 0)
        {
            //DPRINTF("%d has no tokens\n", iline);
            continue;
        }
        else
        {
            //DPRINTF("   %d has %d tokens\n", iline, curline->nstr);
            for(size_t itok = 0; itok < curline->nstr; itok++)
            {   
                char *token = curline->p_sz_toks[itok];
            }
        }
    }
    printf("End\n");
    return;
}


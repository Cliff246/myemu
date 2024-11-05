#include "inc/me_myemu.h"
#include "inc/me_lexer.h"

void print_p_toks_st(p_tok_t token)
{

    DPRINTF("--- p_toks %p ---\n tokenary_len = %lld\n", (void *)&token, token->nstr);

    if (token->nstr == 0)
        return;

    for (int iter = 0; iter < token->nstr; iter++)
    {
        for (char *c = token->p_sz_toks[iter]; *c != 0; c++)
        {
            if (*c == '\n')
                *c = 0;
        }

        DPRINTF(" token = '%s' col = %lld\n", token->p_sz_toks[iter], token->p_u_col[iter]);
    }
}

void print_p_toks_string(p_tok_t token, bool offset)
{
    //DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

    if(token == NULL)
    {
        DPRINTF("failed token%p\n", token);
        return;
    }
    else if (token->nstr == 0)
    {
        //DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

        return;
    }
    else
    {
        //DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

      //  DPRINTF("%d\n", token->nstr);

        size_t last = 0;
        for (int iter = 0; iter < token->nstr; iter++)
        {
            if(offset)
            {
                for(size_t col = last; col < token->p_u_col[iter]; col++)
                {
                    DPRINT(" ");
                }
            }
            else
            {
                DPRINT(" ");
            }
            last = token->p_u_col[iter];

            for (char *c = token->p_sz_toks[iter]; *c != 0; c++)
            {
                if (*c == '\n')
                    *c = 0;
                DPRINTF("%c", *c);
                last++;
                
            }
        }
    }
    DPRINT("\n");
}

p_tok_t malloc_p_toks_st()
{
    p_tok_t tok = (p_tok_t)calloc(1, sizeof(tok_t));
    if (!tok)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    tok->nstr = 0;
    tok->p_sz_toks = NULL;
    tok->p_u_col = NULL;
    return tok;
}

void free_p_toks_st(p_tok_t *token)
{
    const p_tok_t tok = *token;

    if (token == NULL)
    {
        DPRINTF("wtf are you doing not even deleting known data. we need to set your toks data to 0 %s:%d\n", __FILE__, __LINE__);
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
       DPRINTF("freed token %p\n", token);
        // I AM GOING TO TRUST THIS IS FREED CAUSE (IDOIT)
    }
    else
    {
        free(tok);
        DPRINTF("token ptr not defined, freed anyways %s:%d\n", __FILE__, __LINE__);
        // fuck it free it anyways
    }
    *token = NULL;
}

p_tok_t copy_p_toks_st(p_tok_t ref)
{
    if(ref == NULL)
    {
        DPRINT("failed\n");
        exit(1);
    }
    p_tok_t new = malloc_p_toks_st();
    if(new)
    {
        size_t len = ref->nstr;

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
            char **str_copy = (char **)calloc(len, sizeof(char *));
            
            if(!str_copy || !column_copy)
            {
                DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
                exit(1);
            }
            

            for (size_t init_elem = 0; init_elem < len; init_elem++)
            {
                char *source = ref->p_sz_toks[init_elem];
                int sl = strlen(source) + 1;
                char *copy = (char *)calloc(sl, sizeof(char));

                if(!copy)
                {
                    DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
                    exit(1);
                }

                strcpy(copy, source);
               // DPRINTF("|%s| %d\n", source, sl);
                //DPRINTF("|%s| %d\n", copy, strlen(copy) + 1);

                str_copy[init_elem] = copy;
                //DPRINTF("%d:%s\n", init_elem, str_copy[init_elem]);
                column_copy[init_elem] = ref->p_u_col[init_elem];
            }


            new->p_u_col = column_copy;
            new->p_sz_toks = str_copy;
            new->nstr = len;
            //DPRINT("new\n");
           // print_p_toks_string(new, false);
            return new;
        }
    }
    else
    {
        DPRINT("EXIT 1");
        exit(1);
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
    void
        *tmp1 = calloc(length, sizeof(size_t)),
        *tmp2 = calloc(length, sizeof(char *));
    if(!tmp1 || !tmp2)
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
    return;
}

p_tok_t split_str_into_tokens(char *inp, char sep)
{
    if (inp == NULL)
        return NULL;
    p_tok_t p_token_st = malloc_p_toks_st();
    size_t nsep = 1;
    
        for (char *cur = inp + 1, cprev = *inp; *cur != 0; cur++)
        {
            if (*cur == sep && cprev != sep)
            {
                DPRINTF("%c\n", *cur);
                nsep += 1;
            }
            cprev = *cur;
        }
    update_p_toks_st(p_token_st, nsep);

    size_t prev = 0, current = 1, itr = 0;
    size_t *colary = (size_t *)malloc(sizeof(size_t) * nsep);
    if(!colary)
    {
        exit(1);
    }
    char pchar = inp[0];
    for (char *n = inp + 1; *n != 0; n++, current++)
    {
        if (*n == sep && pchar != sep)
        {
            colary[itr] = prev;
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current - prev + 1);
            memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);

            p_token_st->p_sz_toks[itr][current - prev] = 0;
            itr++;
        }
        if (*n != sep && pchar == sep)
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




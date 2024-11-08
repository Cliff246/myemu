#include "me_myemu.h"
#include "me_lexer.h"

#include <string.h>

void print_p_toks_st(p_tok_t token)
{

    printf("--- p_toks %p ---\n tokenary_len = %lld\n", (void *)&token, token->nstr);

    if (token->nstr == 0)
        return;

    for (int iter = 0; iter < token->nstr; iter++)
    {
        for (char *c = token->p_sz_toks[iter]; *c != 0; c++)
        {
            if (*c == '\n')
                *c = 0;
        }

        printf(" token = '%s' col = %lld\n", token->p_sz_toks[iter], token->p_u_col[iter]);
    }
}

void print_p_toks_string(p_tok_t token, bool offset)
{
    // DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

    if (token == NULL)
    {
        DPRINTF("failed token%p\n", token);
        return;
    }
    else if (token->nstr == 0)
    {
        // DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

        return;
    }
    else
    {
        // DPRINTF("LINE %s:%d\n", __FILE__, __LINE__);

        //  DPRINTF("%d\n", token->nstr);

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

    for(size_t i = 0; i < ref->nstr;i++)
    {
        size+= ref->p_u_col[i];
    }
    size+= strlen(ref->p_sz_toks[ref->nstr - 1]);
    char *buffer = calloc(size, sizeof(char));
    if(size == 1)
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
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
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
        // DPRINTF("freed token %p\n", token);
        //  I AM GOING TO TRUST THIS IS FREED CAUSE (IDOIT)
    }
    else
    {
        free(tok);
        // DPRINTF("token ptr not defined, freed anyways %s:%d\n", __FILE__, __LINE__);
        //  fuck it free it anyways
    }
}

char **copy_all_strary(char **src, size_t src_size)
{
    if(!src)
    {
        return NULL;
    }
    char **copy = (char **)calloc(src_size, sizeof(char *));
    if(!copy)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for(size_t iter = 0; iter < src_size; iter++)
    {
        char *refstr = src[iter];
        const size_t reflen = strlen(refstr) + 1;
        char *cpystr = (char *)calloc(reflen, sizeof(char));

        if (!cpystr)
        {
            DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
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
        DPRINTF("failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    const int64_t delta_len = nstop - nstart;
    if (ref->nstr < delta_len || ref->nstr == 0 || delta_len < 0)
    {
        DPRINTF("failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    p_tok_t new = malloc_p_toks_st();
    const size_t new_len = delta_len;

    size_t *column_copy = (size_t *)calloc(new_len, sizeof(size_t));
    if (!column_copy)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
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
        DPRINTF("failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    if (ref->nstr < nsplit || ref->nstr == 0)
    {
        DPRINTF("failed %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    p_tok_t new = malloc_p_toks_st();
    const size_t new_len = ref->nstr - nsplit;

    size_t *column_copy = (size_t *)calloc(new_len, sizeof(size_t));
    if(!column_copy)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    memcpy(column_copy, ref->p_u_col + nsplit, new_len * sizeof(size_t));
    char **str_copy = copy_all_strary(ref->p_sz_toks + nsplit, new_len);

    new->p_u_col = column_copy;
    new->p_sz_toks = str_copy;
    new->nstr = new_len ;

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
            DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }
        memcpy(column_copy, ref->p_u_col, len * sizeof(size_t));
        char **str_copy = copy_all_strary(ref->p_sz_toks, len);

        new->p_u_col = column_copy;
        new->p_sz_toks = str_copy;
        new->nstr = len;
        //DPRINT("new\n");
        //print_p_toks_st(new);
        //print_p_toks_string(new, true);
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
    void
        *tmp1 = calloc(length, sizeof(size_t)),
        *tmp2 = calloc(length, sizeof(char *));
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
            // DPRINTF("%c\n", *cur);
            nsep += 1;
        }
        cprev = *cur;
    }
    update_p_toks_st(p_token_st, nsep);

    size_t prev = 0, current = 1, itr = 0;
    size_t *colary = (size_t *)malloc(sizeof(size_t) * nsep);
    if (!colary)
    {
        exit(1);
    }
    char pchar = inp[0];
    for (char *n = inp + 1; *n != 0; n++, current++)
    {
        if (*n == sep && pchar != sep)
        {
            colary[itr] = prev;
            // DPRINTF("current - prev + 1 %d\n", current - prev + 1);
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
    p_token_st->p_sz_toks[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current + prev + 1);

    memcpy(p_token_st->p_u_col, colary, nsep * sizeof(size_t));
    memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);
    p_token_st->p_sz_toks[itr][current - prev] = 0;

    return p_token_st;
}

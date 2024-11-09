#include "me_myemu.h"
#include "me_commons.h"
#include "me_lexer.h"

#include <stdbool.h>
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
    if (!src)
    {
        return NULL;
    }
    char **copy = (char **)calloc(src_size, sizeof(char *));
    if (!copy)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t iter = 0; iter < src_size; iter++)
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
    if (!column_copy)
    {
        DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
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
            DPRINTF("MALLOC ERROR %s:%d\n", __FILE__, __LINE__);
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

p_tok_t cut_substr_p_tok_t(p_tok_t ref, const char *substr)
{
    size_t newlen = 0;
    bool *toremove_ary = (bool *)calloc(ref->nstr, sizeof(bool));

    for (size_t i = 0; i < ref->nstr; i++)
    {
        if (!cmpstrings(ref->p_sz_toks[i], substr))
        {
            DPRINTF("toks |%s| substr |%s|\n", ref->p_sz_toks[i], substr);
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
    LINE;

    if (inp == NULL)
        return NULL;

    p_tok_t p_token_st = malloc_p_toks_st();
    if (cmpstrings(inp, ""))
    {

        return p_token_st;
    }
    size_t nsep = 0, ntok = 1;

    bool prev_is_sep = char_exist(inp[0], sep);
    char last_sep = inp[0];

    for (char *cur = inp + 1; *cur != 0; cur++)
    {
        char this_sep = *cur;
        bool is_sep = char_exist(this_sep, sep);
      

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
        //DPRINTF("%d %d %d |%c| |%c|\n", is_sep, prev_is_sep, nsep, last_sep, this_sep);
    }
    if(nsep == 0)
    {
       // DPRINTF("%d\n", nsep);
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
    prev_is_sep = char_exist(inp[0], sep);
    //DPRINTF("%s\n", inp);
   // DPRINTF("%d\n", nsep);

    for (char *n = inp ; *n != 0; n++, current++)
    {
        char this_sep = *n;
        bool is_sep = char_exist(this_sep, sep);
        //DPRINTF("|%c| %d %d\n", *n, is_sep, prev_is_sep);
        // DPRINTF("%s\n", n);

        DPRINTF("%d %d %d |%c| |%c|\n", is_sep, prev_is_sep, nsep, last_sep, this_sep);
        if (is_sep && prev_is_sep && last_sep != this_sep)
        {
            // DPRINTF("set %d\n",itr);

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
            // DPRINTF("%s\n", p_token_st->p_sz_toks[itr]);
            // DPRINTF("set %d\n", itr);
            prev = current;

            itr++;
        }
        if (is_sep && !prev_is_sep)
        {
            colary[itr] = prev;
            // DPRINTF("current - prev + 1 %d\n", current - prev + 1);
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current - prev + 1);
            memcpy(p_token_st->p_sz_toks[itr], inp + prev, current - prev);
            // DPRINTF("%s\n", p_token_st->p_sz_toks[itr]);
            p_token_st->p_sz_toks[itr][current - prev] = 0;
            // DPRINTF("set %d\n", itr);
            prev = current;

            itr++;

        }
        
        
        prev_is_sep = is_sep;
        last_sep = this_sep;
    }
    if (char_exist(last_sep, sep))
    {
        DPRINTF("%c\n", last_sep);
        colary[itr] = prev;
        (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], 2 * sizeof(char));
        p_token_st->p_sz_toks[itr][0] = last_sep;
        p_token_st->p_sz_toks[itr][1] = 0;
        // DPRINTF("%s\n", p_token_st->p_sz_toks[itr]);
        // DPRINTF("set %d\n", itr);
    }
    else
    {
    //DPRINTF("%d %d\n", ntok, itr);
        size_t remaining = strlen(inp + prev);
        colary[itr] = prev;
    //DPRINTF("set %d %s\n", itr, inp + prev);
        p_token_st->p_sz_toks[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], remaining  + 1);
        memset(p_token_st->p_sz_toks[itr], 0, remaining + 1);
        memcpy(p_token_st->p_sz_toks[itr], inp + prev, remaining );
    //DPRINTF("|%s|\n", inp + prev);
    }
    memcpy(p_token_st->p_u_col, colary, sizeof(size_t) * (ntok));
   //print_p_toks_st(p_token_st);
    
    free(colary);

    return p_token_st;
}

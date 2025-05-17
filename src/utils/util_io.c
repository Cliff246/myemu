
#include "commons.h"
#include "tokenizer.h"

#include "util_io.h"

size_t get_tokens(p_tok_t **reftok, char *dir)
{
    FILE *pfile = fopen(dir, "r");
    if (pfile)
    {
        const size_t len_history_constant = 1000;
        size_t fchars = 0, numline = 0, len_history = len_history_constant, i_history = 0, tokiter = 0;
        p_tok_t *tokens = (p_tok_t *)calloc(numline, sizeof(p_tok_t));
        size_t tokenalloca = numline + 1;
        char *history = (char *)calloc(len_history + 1, sizeof(char) * 4);
        char c = fgetc(pfile);
        bool gogo = true;
        while (gogo)
        {
            if (c == EOF)
                gogo = false;

            history[i_history++] = c;
            if (c == '\n' || c == EOF)
            {
                trimr(history);
                // size_t historylen = strlen(history);

                p_tok_t tok = split_str_into_tokens(history, " ");
                // print_p_toks_st(tok);
                if (numline + 1 >= tokenalloca)
                {
                    tokenalloca *= 2;
                    // printf("numlines = %d tokensize %d\n",numline, tokenalloca);
                    tokens = (p_tok_t *)REALLOC_SAFE(tokens, sizeof(p_tok_t) * tokenalloca);
                }
                tokens[tokiter++] = tok;
                if (i_history < len_history_constant)
                {
                    memset(history, 0, len_history + 1);
                }
                else
                {
                    len_history = len_history_constant;
                    history = REALLOC_SAFE(history, len_history * 4 + 1 );
                    memset(history, 0, len_history * 4 + 1);
                }
                i_history = 0;
                numline++;
            }
            if (i_history == len_history - 1)
            {
                history = REALLOC_SAFE(history, len_history += (len_history_constant * 4 + 1));
            }
            feof(pfile);
            fchars++;
            c = getc(pfile);
        }

        free(history);
        history = NULL;
        fclose(pfile);
        DPRINTF("closing %s %lld\n", dir, numline);
        *reftok = tokens;
        return numline;
    }
    else
    {
        printf("failed to open document %s\n", dir);
        exit(1);
    }

    return 0;
}



//incomplete
p_hashtable_t read_hashtable_file(char *path)
{

    return NULL;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "inc/me_myemu.h"

#define MAX_FILE_SIZE 1024
#define MAX_LINE_SIZE 1024
#define HASHTABLE_LEN 1024

const char *empty = "";

const char *error_msg_not_enough_args = "";
size_t errorcount = 0;

size_t get_tokens(p_tok_t **reftok, char *dir)
{
    FILE *pfile = fopen(dir, "r");
    if (pfile)
    {
        const size_t len_history_constant = 1000;
        size_t fchars = 0, numline = 0, len_history = len_history_constant, i_history = 0, tokiter = 0;
        p_tok_t *tokens = (p_tok_t *)calloc(numline, sizeof(p_tok_t));

        char *history = (char *)calloc(len_history + 1, sizeof(char));
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
        
                p_tok_t tok = split_str_into_tokens(history, ' ', numline);
                tokens = (p_tok_t *)REALLOC_SAFE(tokens, sizeof(p_tok_t) * (numline + 1));
                tokens[tokiter++] = tok;
                if (i_history < 1000)
                {
                    memset(history, 0, len_history + 1);
                }
                else
                {
                    len_history = 1000;
                    history = REALLOC_SAFE(history, len_history + 1);
                    memset(history, 0, len_history + 1);
                }
                i_history = 0;
                numline++;
            }
            if (i_history == len_history - 1)
            {
                history = REALLOC_SAFE(history, len_history += 1001);
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
    return 0;
}

int compare_with_instructions(char *str)
{
    int index = -1;
    bool found = false;
    char *offset = "    ";
    for (size_t iinst = 0; iinst < ARY_LENGTH(str_instructions); iinst++)
    {

        bool result = cmpstrings(str, str_instructions[iinst]);

        if (result == true)
        {
            found = true;
            index = iinst;
            OFFSET_DPRINTF("compare to instructions %d\n", result);
            OFFSET_DPRINTF("%s %s\n", str, str_instructions[iinst]);
            break;
        }
    }
    if (!found)
    {
        DPRINTF("instruction not found %s:%d\n", __FILE__, __LINE__);
    }
    return index;
}

// STOLEN FROM
// Barmak Shemirani answered Jan 25, 2018 at 5:50:
// https://stackoverflow.com/questions/48433741/counting-minimum-number-of-bytes-required-to-represent-a-signed-integer
// thanks
int getmin(int64_t i)
{
    if (i == (int8_t)(i & 0xFF))
        return 1;
    if (i == (int16_t)(i & 0xFFFF))
        return 2;
    if (i == (int32_t)(i & 0xFFFFFFFF))
        return 4;
    return 8;
}

bool within_hex_codes(char c)
{
    if ((c >= 'a' && c <= 'f') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))
        return true;
    return false;
}

const typedef enum {
    ERROR,
    LITERAL_HEX,
    LITERAL_INT,
    LITERAL_UINT,
    LITERAL_PTR,
    LITERAL_STR,
    LITERAL_CHAR,
    LITERAL_IDENTIFIER,

} literal_id_enum;

int get_argument_type(size_t id_name_len, char *str)
{
    size_t len = strlen(str);

    if (len == 0)
        return -1;

    // ARGUMENT_HEXCODE
    if (len >= 2 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    {

        char *rest = str + 2;
        bool allow = true;

        for (size_t hexchecki = 0; hexchecki < len - 3; hexchecki++)
        {
            if (within_hex_codes(rest[hexchecki]) == false)
            {
                allow = false;
                break;
            }
        }

        // ERROR HEX VALUE NOT VALID
        if (!allow)
        {
            errorcount++;
            return -1;
        }
        char isptr = rest[len - 3];

        if (isptr == 'p')
        {
            DPRINT("THIS ARGUMENT IS A PTR\n");
            return LITERAL_PTR;
        }
        else
        {
            DPRINT("THIS ARGUMENT IS A HEX\n");
            return LITERAL_HEX;
        }
    }
    // INTEGER
    else if (isdigit(str[0]) || str[0] == '-' || str[0] == '+' || str[0] == 'u' || str[0] == 'U')
    {
        bool hasoffset = false;
        bool issigned = true;
        if ((str[0] == '-' || str[0] == '+') && len > 1)
        {
            hasoffset = true;
            issigned = true;
        }
        if ((str[0] == 'u' || str[0] == 'U') && len > 1)
        {
            issigned = false;
        }
        bool allow = true;

        if (hasoffset)
        {
            for (size_t intchecki = 1; intchecki < len - 1; intchecki++)
            {
                if (!isdigit(str[intchecki]))
                {
                    allow = false;
                    break;
                }
            }
        }
        else
        {
            for (size_t intchecki = 0; intchecki < len; intchecki++)
            {
                if (!isdigit(str[intchecki]))
                {
                    allow = false;
                    break;
                }
            }
        }

        if (allow)
        {
            if (issigned)
            {
                DPRINT("THIS ARGUMENT IS A INT\n");
                return LITERAL_INT;
            }
            else
            {
                DPRINT("THIS ARGUMENT IS A UINT\n");
                return LITERAL_UINT;
            }
        }
        // integer is not valid
        else
        {
            errorcount++;
            return ERROR;
        }
    }
    else if (str[0] == '"' && str[len - 1] == '"')
    {
        DPRINT("THIS ARGUMENT IS A STRING TYPE\n");
        return LITERAL_STR;
    }

    else if (len == 3 && str[0] == 39 && str[2] == 39)
    {
        return LITERAL_CHAR;
    }
    // ARGUMENT CUSTOM literal

    return ERROR;
}

size_t decode_nbytes_for_argument(int argtype, char *str)
{
    size_t len = strlen(str);

    switch (argtype)
    {
    case LITERAL_INT:
        return getmin(atol(str));
    case LITERAL_HEX:
        return len - 2;
    case LITERAL_PTR:
        return len - 3;
    case LITERAL_STR:
        return len - 2;
    case LITERAL_UINT:
        return getmin(atol(str + 1));
    case LITERAL_CHAR:
        return 1;
    default:
        // cringe
        printf("cannot get bytes from argument\n");
        exit(1);
        break;
    }
    return 0;
}

void convert_token_to_bytes(char *str, int type, char *bytes, size_t bytes_len)
{
    if (str && bytes && type >= 0 && type < LITERAL_IDENTIFIER && bytes_len > 0)
    {
        size_t len = strlen(str);
        size_t bytesiter = 0;

        if (type == LITERAL_STR)
        {
            if (str[0] == '"' && str[len - 1] == '"' && bytes_len >= len - 3)
            {
                for (size_t striter = 1; striter < len - 1; striter++)
                {
                    bytes[bytesiter++] = str[striter];
                }
                bytes[bytesiter++] = 0;
                return;
            }
            else
            {
                DPRINT("NOT A STRING\n");
                exit(1);
            }
        }
        if (type == LITERAL_HEX)
        {
            DPRINTF("LITERAL HEX->%s\n", str);
            char *hexpart = str + 2;
            size_t num = (size_t)strtol(hexpart, NULL, 16);
            size_t len = getmin(num);
            for (size_t i = 0; i < len; i++)
            {
                int x = (num >> (8 * i)) & 0xff;
                bytes[bytesiter++] = (char)x;
            }
            return;
        }
        if (type == LITERAL_PTR)
        {
            char hexpart[len - 3];
            strncpy(hexpart, str + 2, len - 3);
            DPRINTF("LITERAL PTR->%s\n", hexpart);
            size_t num = (size_t)strtol(hexpart, NULL, 16);
            size_t len = getmin(num);
            for (size_t i = 0; i < len; i++)
            {
                int x = (num >> (8 * i)) & 0xff;
                bytes[bytesiter++] = (char)x;
            }
            return;
        }
        if (type == LITERAL_CHAR)
        {
            if (str[0] == 0x27 && str[2] == 0x27 && bytes_len == 1 && len == 3)
            {
                printf("WHAT\n");

                bytes[0] = str[1];
            }
            else
            {
                printf("WHAT\n");
            }
            return;
        }
    }
    else
    {
        DPRINT("CANNOT CONVERT TOKEN TO BYTES");
        exit(1);
    }
}

void *make_reference(char *key, size_t orgin, int argtype)
{
    p_reference_t refptr = (p_reference_t)malloc(sizeof(reference_t));
    refptr->p_sz_key = key;
    refptr->norgin = orgin;
    refptr->argtype = argtype;
    return (void *)refptr;
}

p_reference_t get_reference(void *ptr)
{
    p_reference_t refptr = (p_reference_t)ptr;
    return refptr;
}

// does not free key
void free_reference(void *ptr)
{
    p_reference_t ref = ptr;
    free(ref);
}

void stage1(p_context_t context, p_program_t program)
{
    size_t lines = context->nlines;
    p_tok_t *tokens = context->p_tokens;
    p_hashtable_t identifier_table = new_hash_table(HASHTABLE_LEN, free_reference);


    for (size_t iter_line = 0; iter_line < lines; iter_line++)
    {
        p_tok_t line = tokens[iter_line];
        print_p_toks_st(line);
    }
}

void stage2(p_tok_t *tokens, size_t lines)
{
}

void assemble(char *dir, size_t size)
{

    p_tok_t *tokens = NULL;
    size_t lines = get_tokens(&tokens, dir);

    DPRINTF("%lld\n", lines);

    size_t asmerr_ary_len = 1;
    const size_t increment = 1000;

    char *program_ptr = (char *)malloc(sizeof(char) * increment);
    p_asmerr_t *asmerr_array = (p_asmerr_t *)calloc(asmerr_ary_len, sizeof(asmerr_t));

    program_t program = {.p_program = program_ptr, .n_memorysize = increment, .n_increment = increment, .n_used = 0};
    context_t context = {.nlines = lines, .p_identifier_table = NULL, .p_sz_fname = dir, .p_tokens = tokens};
    stage1(&context, &program);

    asmerr_array[0] = NULL;

    DPRINTF("%lld errors\n", errorcount);
    DPRINT("End\n");
    return;
}

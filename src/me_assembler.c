#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "me_commons.h"
#include "me_source.h"
#include "me_assembler.h"

#define MAX_FILE_SIZE 1024
#define MAX_LINE_SIZE 1024

const char *empty = "";

const char *error_msg_not_enough_args = "";


typedef struct tokens
{
    char * *p_sz_toks;
    size_t *p_u_col;
    size_t nstr, line;
}tok_t, *p_tok_t;

typedef struct custom_literal
{
    char *value;
    char *identifier_copy;
    size_t id, value_len;
    int value_type, value_pos;
}custlit_t, *p_custlit_t;

typedef struct assembler_error
{
    size_t line;
    size_t column;
    int code;
    char *file;
    char *msg;
    
}asmerr_t, *p_asmerr_t;

void print_p_toks_st(p_tok_t token)
{

    DPRINTF("--- p_toks %p ---\n line_num =%d, tokenary_len = %d\n", &token, token->line, token->nstr);
    
    if(token->nstr == 0)
        return;
    
    for(int iter = 0; iter < token->nstr; iter++)
    {   
        for(char * c = token->p_sz_toks[iter]; *c != 0; c++)
        {
            if(*c == '\n')
                *c = 0;
        }

        DPRINTF(" token = '%s' col = %d\n", token->p_sz_toks[iter], token->p_u_col[iter]);
    }
}

void print_p_cuslit_st(p_custlit_t cuslit)
{
    if(cuslit)
        DPRINTF("custom literal -> %s | id = %d | value_len = %d value_type %d value_pos %d\n", cuslit->identifier_copy, cuslit->id, cuslit->value_len, cuslit->value_type, cuslit->value_pos);
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

p_custlit_t malloc_p_custlit_st(char * identfier)
{
    if(identfier != NULL)
    {
        static size_t _id = 0;
        p_custlit_t lit = (p_custlit_t)calloc(1, sizeof(custlit_t));
        if(!lit)
        {
            DPRINTF("big oops on calloc %s %d\n", __FILE__,__LINE__);
            exit(1);
        }
        size_t identfier_len = strlen(identfier);
        lit->id = _id++;
        //I AM DECIDING TO COPY DATA TO A NEW MEMORY ADDRESS
        //THIS WILL ALLOW FOR DATA TO BE DELETED MORE SAFELY
        // EVEN THOUGH ITS NOT NECCESSARY ITS SAFER AND ALLOWS FOR EASIER WORK IN THE FUTURE

        char * identfier_copy_tmp = (char *)calloc(identfier_len + 1, sizeof(char));
        errno_t err = strcpy_s(identfier_copy_tmp, identfier_len + 1, identfier);
        if(err != 0)
        {
            DPRINTF("HUGE STRCPY_S ERROR IN %s:%d", __FILE__, __LINE__);
            exit(1);
        }
        lit->identifier_copy = identfier_copy_tmp;
        lit->value = NULL;
        lit->value_len = 0;
        lit->value_type = -1;
        lit->value_pos = 0;
        return lit;
    }
    else
    {
        DPRINT("MALLOC P_CUSLIT_ST FAILED DUE TO IDENTFIER BEING NULL\n");
        exit(1);
    }
}

void update_p_custlit_st(p_custlit_t *custlit_ary, size_t custlit_ary_len, p_custlit_t custlit, int type, char *bytes, size_t nbytes)
{
    if(custlit_ary && custlit && custlit_ary_len > 0 )
    {
        if(nbytes == 0 && !bytes)
        {
            custlit->value = NULL;
            custlit->value_len = 0;
            custlit->value_pos = -1;
            custlit->value_type = type;
        }
        else if(nbytes > 0 && !bytes)
        {
            DPRINT("ERROR CUSTOM LITERAL INPUT CONDITION MUST HAVE NBYTES > 0 AND BYTES BE A VALID PTR \n");
            exit(1);
        }
        size_t offset = 0;
        if(custlit->value != NULL)
        {
            DPRINT("ERROR CUSTOM LITERAL IS ALREADY SET\n");
            exit(1);
        }
        char *temp = (char *)calloc(nbytes, sizeof(char));
        if(!temp)
        {
            DPRINTF("big oops on calloc %s %d\n", __FILE__,__LINE__);
            exit(1);
        }
        errno_t err1 = memcpy_s(temp, nbytes, bytes, nbytes);
        if(err1 != 0)
        {
            DPRINTF("HUGE ERROR BIG MASSIVE MEMCPY_S ERROR WE DYING %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }
        bool foundme = false;
        for(size_t iter = 0; iter < custlit_ary_len; iter++)
        {    
            if(custlit->id == custlit_ary[iter]->id)
            {
                custlit->value_type = type;
                custlit->value = temp;
                custlit->value_len = nbytes;
                custlit->value_pos = offset;
                foundme = true;
            }
            if(foundme)
                custlit_ary[iter]->value_pos = offset++;
            offset += custlit_ary[iter]->value_len;
            
        }
        if(foundme == false)
        {
            custlit->value_type = type;
            custlit->value = temp;
            custlit->value_len = nbytes;
            custlit->value_pos = offset + 1;
        }
    }
    else
    {
        DPRINT("ERROR CUSTOM LITERAL INPUT CONDITION MUST BE ALL VALID \n");
        exit(1);
    }
}


void free_p_custlit_st(p_custlit_t lit)
{
    if(lit != NULL)
    {
        free(lit->identifier_copy);
        free(lit); 
    }
}

void free_p_toks_st(p_tok_t tok)
{
    if(tok != NULL)
    {
        for(size_t i = 0; i < tok->nstr; i++)
        {
            if(tok->p_sz_toks[i])
                free(tok->p_sz_toks[i]);
        }
        free(tok->p_sz_toks);
        free(tok);
    }
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
        *tmp2 = calloc(length, sizeof(char *));
    ptok->p_u_col = (size_t*)tmp1;
    ptok->p_sz_toks = (char **)tmp2;
    ptok->nstr = length;
    ptok->line = nline;
    for(size_t init_elem = 0; init_elem < ptok->nstr; init_elem++)
    {
        if(ptok->p_sz_toks[init_elem] != NULL)
            continue;
        else
        {
            char * str = (char *)calloc(1, sizeof(char));
            if(!str)
                exit(1);
            ptok->p_sz_toks[init_elem] = str;

        }
    }    
    return;
}


p_tok_t split_str_into_tokens(char * inp, char sep, size_t line)
{
    if(inp == NULL)
        return NULL;
    p_tok_t p_token_st = malloc_p_toks_st();
    size_t inp_length = strnlen(inp, MAX_FILE_SIZE);
    size_t nsep = 1;
    errno_t err1, err2, err3;
    if(inp_length == 0)
    {
        update_p_toks_st(p_token_st, 0, line);
        return p_token_st;    
    }
    else
    {
        for(char * cur = inp + 1, cprev = *inp; *cur != 0; cur++)
        {
            if(*cur == sep && cprev != sep)
            {
                DPRINTF("%c\n", *cur);
                nsep+=1;   
            }
            cprev = *cur;
        }
    }
    update_p_toks_st(p_token_st, nsep, line);

    size_t prev = 0, current = 1, itr = 0;
    size_t *colary = (size_t*)malloc(sizeof(size_t) * nsep);

    char pchar = inp[0];
    for(char * n = inp + 1; *n != 0; n++, current++)
    {
        if(*n == sep && pchar != sep)
        {
            colary[itr] = prev;
            (p_token_st->p_sz_toks)[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], current - prev + 1);
            err1 = memcpy_s(p_token_st->p_sz_toks[itr],current - prev + 1,  inp + prev, current - prev);
            if(err1 != 0)
            {
                DPRINTF("HUGE ERROR BIG MASSIVE MEMCPY_S ERROR WE DYING %s:%d\n", __FILE__, __LINE__);
                exit(1);
            }
            
            p_token_st->p_sz_toks[itr][current - prev] = 0;
            itr++;
        }
        if(*n != sep && pchar == sep)
            prev = current;
        pchar = *n;
    }   
    colary[nsep - 1] = prev;
    p_token_st->p_sz_toks[itr] = REALLOC_SAFE(p_token_st->p_sz_toks[itr], (current + prev + 1) * sizeof(char));
   
    err2 = memcpy_s(p_token_st->p_u_col,nsep * sizeof(size_t), colary, nsep * sizeof(size_t));
    if(err2 != 0)
    {
        DPRINTF("HUGE ERROR BIG MASSIVE MEMCPY_S ERROR WE DYING %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    err3 = memcpy_s(p_token_st->p_sz_toks[itr],current - prev + 1 , inp + prev, current - prev);
    if(err3 != 0)
    {
        DPRINTF("HUGE ERROR BIG MASSIVE MEMCPY_S ERROR WE DYING %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    p_token_st->p_sz_toks[itr][current - prev] = 0;
    return p_token_st;
}

int compare_with_instructions(char * str)
{
    int index = -1;
    bool found = false;
    char * offset = "    "; 
    for(size_t iinst = 0; iinst < ARY_LENGTH(str_instructions); iinst++)
    {

        bool result = cmpstrings(str, str_instructions[iinst]);

        if(result == true)
        {
            found = true;
            index = iinst;
            OFFSET_DPRINTF("compare to instructions %d\n", result);
            OFFSET_DPRINTF("%s %s\n",str, str_instructions[iinst]);
            break;
        }
    }
    return index;
}

//STOLEN FROM 
//Barmak Shemirani answered Jan 25, 2018 at 5:50: 
//https://stackoverflow.com/questions/48433741/counting-minimum-number-of-bytes-required-to-represent-a-signed-integer
//thanks 
int getmin(int64_t i)
{
    if(i == (int8_t)(i & 0xFF))
        return 1;
    if(i == (int16_t)(i & 0xFFFF))
        return 2;
    if(i == (int32_t)(i & 0xFFFFFFFF))
        return 4;
    return 8;
}



size_t get_tokens(p_tok_t **reftok,char * dir)
{
    FILE *pfile = fopen(dir, "r");
    if(pfile)
    {
        size_t fchars = 0;
        size_t numline = 0;
        p_tok_t *tokens = (p_tok_t*)calloc(numline, sizeof(p_tok_t));

        size_t len_history = 1000, i_history = 0, tokiter = 0;
        char * history =(char*)calloc(len_history + 1, sizeof(char));
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
                size_t historylen = strlen(history);
                p_tok_t tok = split_str_into_tokens(history, ' ', numline);
                tokens = (p_tok_t *)REALLOC_SAFE(tokens, sizeof(p_tok_t) * (numline + 1));
                tokens[tokiter++] = tok; 
                if(i_history < 1000)
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

bool within_hex_codes(char c)
{
    if((c >= 'a' && c <= 'f') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))
        return true;
    return false;
}
#define ERROR -1
#define LITERAL_HEX 0
#define LITERAL_INT 1
#define LITERAL_UINT 2
#define LITERAL_PTR 3
#define LITERAL_STR 4
#define LITERAL_CHAR 5 
#define LITERAL_IDENTIFIER 6

int get_argument_type(p_custlit_t *id_name_ary, size_t id_name_len, char * str)
{
    size_t len = strlen(str);
    
    if(len == 0)
        return -1;

    //ARGUMENT_HEXCODE
    if(len >= 2 && str[0] == '0' && (str[1] == 'x'||str[1] == 'X'))
    {
       
        char* rest = str + 2;
        bool allow = true;
       
        for(size_t hexchecki = 0; hexchecki < len - 3; hexchecki++)
        {
            if(within_hex_codes(rest[hexchecki]) == false)
            {
                allow = false;
                break;
            }
        }
        
        
        //ERROR HEX VALUE NOT VALID
        if(!allow)
        {
            errorcount++;
            return -1;
        }
        char isptr = rest[len -3 ];
       
        if(isptr == 'p')
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
    //INTEGER
    else if (isdigit(str[0]) || str[0] == '-' || str[0] == '+')
    {
        bool negative = false;
        if(str[0] == '-' && len > 1)
            negative = true;
        if(str[0] == '+' && len > 1)
            negative = false;
        bool allow = true;
        for(size_t intchecki = 1; intchecki < len - 1; intchecki++)
        {
            if(!isdigit(str[intchecki]))
            {
                allow =false;
                break;
            }
        }
        if(allow)
        {
            DPRINT("THIS ARGUMENT IS A INT\n");
            return LITERAL_INT;
        }
        //integer is not valid
        else
        {
            errorcount++;
            return ERROR;
        }
    }
    else if(str[0] == '"' && str[len - 1] == '"')
    {
        DPRINT("THIS ARGUMENT IS A STRING TYPE\n");
        return LITERAL_STR;
        
        
        








    }
    
    else if(len == 3 && str[0] == 39 && str[2] == 39)
    {
        return LITERAL_CHAR;

    }
    //ARGUMENT CUSTOM literal
    else
    {
        if(len > 0)
        {
            if(str[0] == '$')
            {
                bool allow = true;
                for(size_t checkitr = 1; checkitr < len; checkitr++)
                {
                    if(!isalnum(str[checkitr]) && str[checkitr] != '_')
                    {
                        allow = false;
                        break;
                    }
                }      
                if(allow)
                {
                    int id = 0;
                    bool found = false;
                    for(size_t namecheck = 0; namecheck < id_name_len; namecheck++)
                    {
                        DPRINTF("ID NAME ARY %s\n",id_name_ary[namecheck]->identifier_copy);
                        p_custlit_t current_cuslit = id_name_ary[namecheck];
                        bool result = cmpstrings(current_cuslit->identifier_copy , str);
                        if(result == true)
                        {
                            id = namecheck;
                            found = true;
                            break;
                        }
                    }
                    if(found)
                    {
                        DPRINT("THIS ARGUMENT IS A CUSTOM TYPE\n");
                        return LITERAL_IDENTIFIER + id;
                    }
                    else
                    {
                        errorcount++;
                        return ERROR;
                    }
                }
                else 
                {
                    //FIRST INDEX MUST BE int
                    errorcount++;
                    return ERROR;
                }
            }
        }
    }
}

size_t decode_nbytes_for_argument(int argtype, char *str)
{
    size_t len = strlen(str);
    if(argtype == LITERAL_INT)
        return getmin(atol(str));
    else if(argtype == LITERAL_HEX)
        return len - 2;
    else if(argtype == LITERAL_PTR)
        return len - 3;
    else if(argtype == LITERAL_STR)
        return len - 2;
    else if(argtype == LITERAL_UINT)
        return getmin(atol(str + 1));
    else if(argtype == LITERAL_CHAR)
        return 1;
    else 
    {
        printf("cannot get bytes from argument\n");
        exit(1);
    }
    return 0;
}

void convert_token_to_bytes(char *str, int type, char *bytes, size_t bytes_len)
{
    if(str && bytes && type >= 0 && type < LITERAL_IDENTIFIER && bytes_len > 0)
    {
        size_t len = strlen(str);
        size_t bytesiter = 0;

        if(type == LITERAL_STR)
        {
            if(str[0] == '"' && str[len - 1] == '"' && bytes_len >= len - 3)
            {
                for(size_t striter = 1; striter < len - 1; striter++)
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
        if(type == LITERAL_HEX)
        {
            DPRINTF("LITERAL HEX->%s\n", str);
            char *hexpart = str + 2;
            size_t num = (size_t)strtol(hexpart, NULL, 16);
            size_t len = getmin(num);
            for(size_t i = 0; i < len; i++)
            {
                int x = (num >> (8*i)) & 0xff;
                bytes[bytesiter++] = (char)x;
            }
            return;
        }
        if(type == LITERAL_PTR)
        {
            char hexpart[len - 3];
            strncpy(hexpart, str + 2, len - 3);
            DPRINTF("LITERAL PTR->%s\n", hexpart);
            size_t num = (size_t)strtol(hexpart, NULL, 16);
            size_t len = getmin(num);
            for(size_t i = 0; i < len; i++)
            {
                int x = (num >> (8*i)) & 0xff;
                bytes[bytesiter++] = (char)x;
            }
            return;
        }
        if(type == LITERAL_CHAR)
        {
            if(str[0] == 0x27 && str[2] == 0x27 && bytes_len == 1 && len == 3)
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



void assemble(char * dir, size_t size)
{
    

    DPRINTF("%d\n", getmin(100));

    
    p_tok_t *tokens = NULL;
    size_t lines = get_tokens(&tokens,dir); 
    
    DPRINTF("%d\n", lines);
    
    size_t memorysize = 1, id_name_ary_length = 0, asmerr_ary_len = 0;
    char * program = (char * )malloc(1);

    p_custlit_t *id_name_array = (p_custlit_t *)calloc(1, sizeof(custlit_t));
    p_asmerr_t *asmerr_array = (p_asmerr_t *)calloc(1, sizeof(asmerr_t));
    DPRINTF("%d\n",(id_name_ary_length));

    for(size_t iline = 0; iline < lines; iline++)
    {
        p_tok_t curline = tokens[iline];
        LINE;
        //EMPTY LINE
        if(curline->nstr == 0)
        {
            DPRINTF("%d has zero tokens\n", iline);
            continue;
        }
        //LINE WITH VALUES
        else
        {
            bool iscomment = false, isidentifier = false;
            int cmp = -1;
            char * token = curline->p_sz_toks[0];
            if(token[0] == '#')
                iscomment = true;
            if(token[0] == '$')
                isidentifier = true;
            DPRINTF("%d has %-4d tokens\n", iline, curline->nstr);
            cmp = compare_with_instructions(token);
            //TEST if in instruction list
            //This will test all possible states
            if(cmp >= 0)
            {
                size_t nargs = len_instructions[cmp];
                //ENOUGH ARGUMENTS
                print_p_toks_st(curline);
                DPRINTF("nargs == %d line has %d tokens\n", nargs, curline->nstr);

                if(nargs <= curline->nstr)
                {
                    char *psz_args[nargs];
                    int arg_types[nargs];

                    for(size_t itok = 1; itok < curline->nstr; itok++)
                    {   
                        char *argtoken = curline->p_sz_toks[itok];
                        DPRINTF("    %p\n",  argtoken);
                        
                        psz_args[itok - 1] = argtoken;
                        DPRINTF("    %s\n", psz_args[itok - 1]);
                    }
                    for(size_t iarg = 0; iarg < nargs; iarg++)
                    {
                        arg_types[iarg] = get_argument_type(id_name_array, id_name_ary_length, curline->p_sz_toks[iarg]);
                        DPRINTF("    argument types %d\n",arg_types[iarg]);
                    }
                    continue;

                }
                //NOT ENOUGH ARGUMENTS ERROR
                else 
                {
                    errorcount++;
                    DPRINTF("not enough arguments (has %d and needs %d)\n", curline->nstr, nargs);
                    continue;
                }
                
            }
            // IS COMMENT

            else if(iscomment == true)
            {
                DPRINT("IS COMMENT\n");
                for(size_t itok = 0; itok < curline->nstr; itok++)
                {   
                    char * argtoken = curline->p_sz_toks[itok];
                    DPRINTF("%s ", argtoken);
                }
                DPRINT("\n");
                continue;
            }

            //IS IDENTIFIER
            else if(isidentifier == true)
            {   
                DPRINT("IS IDENTFIER\n");
                if(curline->nstr == 2)
                {
                    char offset[] = "    ";
                    OFFSET_DPRINTF("%d\n",(id_name_ary_length));
                    OFFSET_DPRINTF("%p\n",id_name_array);
                    bool allow = true;
                    for(size_t identfiers = 0; identfiers < id_name_ary_length; identfiers++)
                    {
                        bool result = cmpstrings(token, id_name_array[identfiers]->identifier_copy);
                        if(result == true)
                        {
                            
                            DPRINTF("TOKEN ALLREADY EXISTS %s %s\n",token, id_name_array[identfiers]->identifier_copy);
                            errorcount++;
                            allow = false;
                            break;
                        }
                    }
                    if(!allow)
                        continue;
                    id_name_array = (p_custlit_t *)REALLOC_SAFE(id_name_array, (id_name_ary_length + 1) * sizeof(p_custlit_t));
                    id_name_array[id_name_ary_length] = malloc_p_custlit_st(token);
                    size_t numberofbytes = 0;
                    char *bytes = malloc(sizeof(char) * (numberofbytes + 1));
                    char *arg = curline->p_sz_toks[1];
                    int argtype = get_argument_type(id_name_array, id_name_ary_length, arg);
                    numberofbytes += decode_nbytes_for_argument(argtype, arg);
                    bytes = (char *)REALLOC_SAFE(bytes, (numberofbytes + 1) * sizeof(char));
                    convert_token_to_bytes(arg, argtype, bytes, numberofbytes);
                    update_p_custlit_st(id_name_array, id_name_ary_length + 1, id_name_array[id_name_ary_length], argtype, bytes, numberofbytes);
                    id_name_ary_length++;
                    continue;
                }
                else
                {
                    DPRINT("NOT ENOUGH OR TOO MANY ARGUMENT\n");
                    errorcount++;
                    continue;
                }
            }
            //TODO ERROR CHECKING
            else
            {
                errorcount++;
                DPRINTF("Unknown Instruction -> %s\n", token);
                continue;
            }
            
        }
    }
    DPRINTF("%d errors\n", errorcount);
    DPRINT("End\n");
    return;
}


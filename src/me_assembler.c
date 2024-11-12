#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "me_constants.h"
#include "me_myemu.h"
#include "me_lexer.h"

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
        size_t tokenalloca = numline + 1;
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

                p_tok_t tok = split_str_into_tokens(history, " ");
                // print_p_toks_st(tok);
                if (numline + 1 >= tokenalloca)
                {
                    tokenalloca *= 2;
                    // printf("numlines = %d tokensize %d\n",numline, tokenalloca);
                    tokens = (p_tok_t *)REALLOC_SAFE(tokens, sizeof(p_tok_t) * tokenalloca);
                }
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
    // DPRINT("INSTRUCTION:\n");
    for (size_t iinst = 0; iinst < ARY_LENGTH(str_instructions); iinst++)
    {

        bool result = cmpstrings(str, str_instructions[iinst]);

        if (result == true)
        {
            found = true;
            index = iinst;
            // OFFSET_DPRINTF("compare to instructions:\n      %s %s\n", str, str_instructions[iinst]);
            break;
        }
    }
    if (!found)
    {
        // DPRINTF("instruction not found %s:%d\n", __FILE__, __LINE__);
    }
    return index;
}

// STOLEN FROM
// Barmak Shemirani answered Jan 25, 2018 at 5:50:
// https://stackoverflow.com/questions/48433741/counting-minimum-number-of-bytes-required-to-represent-a-signed-integer
// thanks
int getmin(uint64_t i)
{
    if (i == (uint8_t)(i & 0xFF))
        return 1;
    if (i == (uint16_t)(i & 0xFFFF))
        return 2;
    if (i == (uint32_t)(i & 0xFFFFFFFF))
        return 4;
    return 8;
}

bool within_hex_codes(char c)
{
    if ((c >= 'a' && c <= 'f') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9'))
        return true;
    return false;
}

size_t decode_nbytes_for_argument(argument_type_t argtype, char *str)
{
    size_t len = strlen(str);
    if (argtype == e_sint)
        return getmin(atol(str));
    else if (argtype == e_hex)
        return len - 2;
    else if (argtype == e_ptr)
        return len - 3;
    else if (argtype == e_uint)
        return getmin(atol(str + 1));
    else if (argtype == e_char)
        return 1;
    else
    {
        printf("cannot get bytes from argument\n");
        exit(1);
    }
    return 0;
}

argument_type_t get_argument_type(char *str)
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
            return e_ptr;
        }
        else
        {
            return e_hex;
        }
    }
    // INTEGER
    else if (isdigit(str[0]) || str[0] == '-' || str[0] == '+' || str[0] == 'u' || str[0] == 'U')
    {
        // DPRINTF("%s\n", str);
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
            hasoffset = true;
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
                return e_sint;
            }
            else
            {
                return e_uint;
            }
        }
        // integer is not valid
        else
        {
            errorcount++;
            return e_error;
        }
    }
    else if (len == 3 && str[0] == 39 && str[2] == 39)
    {
        return e_char;
    }
    // ARGUMENT CUSTOM literal

    return e_error;
}

void print_argumenttype(argument_type_t type)
{
    switch (type)
    {
    case e_char:
        DPRINT("THIS ARGUMENT IS A CHAR\n");

        break;
    case e_hex:
        DPRINT("THIS ARGUMENT IS A HEX\n");

        break;
    case e_ptr:
        DPRINT("THIS ARGUMENT IS A PTR\n");

        break;
    case e_sint:
        DPRINT("THIS ARGUMENT IS A INT\n");

        break;
    case e_uint:
        DPRINT("THIS ARGUMENT IS A UINT\n");

        break;
    case e_error:
        DPRINT("UNKNOWN ARGUMENT\n");
        break;
    }
}

size_t convert_token_to_bytes(char *str, argument_type_t type, char *bytes, size_t bytes_len)
{
    if (str && bytes && bytes_len > 0)
    {
        size_t len = strlen(str);
        size_t bytesiter = 0;
        memset(bytes, 0, bytes_len);

        if (type == e_hex)
        {
            // DPRINTF("LITERAL HEX->%s\n", str);
            char *hexpart = str + 2;
            size_t num = (size_t)strtol(hexpart, NULL, 16);
            size_t len = getmin(num);
            // LINE;
            // DPRINTF("%d\n", len);
            for (size_t i = 0; i < len; i++)
            {
                unsigned char x = (num >> (8 * i)) & 0xff;
                bytes[bytesiter++] = (unsigned char)x;
            }

            return bytesiter;
        }
        if (type == e_ptr)
        {
            // DPRINTF("LITERAL PTR->%s\n", hexpart);
            size_t num = (size_t)strtol(str + 2, NULL, 16);
            size_t len = getmin(num);
            for (size_t i = 0; i < len; i++)
            {
                int x = (num >> (8 * i)) & 0xff;
                bytes[bytesiter++] = (char)x;
            }
            return bytesiter;
        }
        if (type == e_char)
        {
            // DPRINTF("%s\n", str);

            if (str[0] == 0x27 && str[2] == 0x27 && bytes_len == 1 && len == 3)
            {
                // DPRINTF("%c\n", str[1]);

                bytes[0] = str[1];
            }
            else
            {
                exit(1);
            }
            return 1;
        }
        if (type == e_sint || type == e_uint)
        {
            // DPRINTF("LITERAL INT->%s\n", str);
            size_t num = 0;
            size_t len = 0;
            if (isdigit(str[0]))
            {
                num = (size_t)strtol(str, NULL, 10);
                len = getmin(num);
            }
            if (str[0] == 'u' || str[0] == 'U')
            {
                num = (size_t)strtol(str + 1, NULL, 10);
                len = getmin(num);
            }
            else
            {
                num = (size_t)strtol(str + 1, NULL, 10);
                len = getmin(num);
                num = num << 1;
                num = SETBIT(num, 1);
                // print_bin(num, 8, 1);
            }

            for (size_t i = 0; i < len; i++)
            {
                int x = (num >> (8 * i)) & 0xff;
                // print_bin(x, 8, 1);
                bytes[bytesiter++] = (char)x;
            }

            // DPRINTF("size %d\n", len);
            return len;
        }
    }
    else
    {
        DPRINT("CANNOT CONVERT TOKEN TO BYTES");
        exit(1);
    }
}

void *make_reference(p_section_t section)
{
    p_reference_t ref = (p_reference_t)calloc(1, sizeof(reference_t));
    if (ref)
    {
        ref->sect = section;

        return (void *)ref;
    }
}

p_reference_t get_reference(void *ptr)
{
    p_reference_t ref = (p_reference_t)ptr;
    return ref;
}

void free_reference(void *ptr)
{
    p_reference_t ref = (p_reference_t)ptr;
    free(ref);
    ref = NULL;
}

p_memseg_t make_memsegment(char *bytes, size_t start, size_t end)
{
    const int size = end - start;
    if (size <= 0)
    {
        DPRINTF("memsegment size failure %s:%d\n", __FILE__, __LINE__);
        return NULL;
    }
    p_memseg_t segment = (p_memseg_t)calloc(1, sizeof(memseg_t));
    char *copy = (char *)calloc(size, sizeof(char));
    if (segment && copy)
    {
        memcpy(copy, bytes, size);
        segment->mem_bytes = copy;
        segment->memstart = start;
        segment->memstop = end;
        return segment;
    }
    else
    {
        DPRINTF("memsegment size failure %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
}

void print_memsegment(p_memseg_t memseg)
{
    int size = memseg->memstop - memseg->memstart;
    if (size < 0)
    {
        printf("memsegment size failure %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    printf("start:%llu stop:%llu\n", memseg->memstart, memseg->memstop);
    for (int i = 0; i < size; i++)
    {
        print_hex(memseg->mem_bytes[i], true);
    }
}

void free_memsegment(p_memseg_t memseg)
{
    free(memseg->mem_bytes);
    free(memseg);
    memseg = NULL;
}

p_section_t new_section(section_type_t type, char *key)
{
    static int id = 1;
    const int initlen = 0, initalloc = 10;
    p_section_t sect = (p_section_t)calloc(1, sizeof(section_t));
    p_tok_t *list = (p_tok_t *)calloc(initalloc, sizeof(p_tok_t));
    if (sect && list)
    {
        sect->len = initlen;
        sect->allocd = initalloc;
        sect->toks = list;
        sect->sectype = type;
        sect->initalized = 0;
        sect->id = id++;
        sect->keystr = key;
        return sect;
    }
    return NULL;
}
// copies data
void update_section(p_section_t section, p_tok_t tok)
{
    if (section)
    {

        if (section->len >= section->allocd - 1)
        {
            section->allocd *= 2;
            section->toks = REALLOC_SAFE(section->toks, section->allocd * sizeof(p_tok_t));
        }
        // LINE;
        p_tok_t newtok = copy_p_toks_st(tok);
        // DPRINT("--newtok--\n");
        // print_p_toks_string(newtok);
        //        print_p_toks_string(tok, false);

        section->toks[section->len] = newtok;
        section->len = section->len + 1;
    }
    else
    {
        DPRINTF("SECTION NOT DEFINED %p %p\n", section, tok);
        exit(1);
    }
}

void free_section(p_section_t section)
{
    for (int i = 0; i < section->len; i++)
    {
        // DPRINT("FREEING\n");
        // print_p_toks_st(section->toks[i]);
        free_p_toks_st(section->toks[i]);
    }
    free(section->keystr);
    free(section->toks);
    free(section);
}

void print_section(p_section_t section)
{
    if (!section)
    {
        DPRINTF("section failed %p\n", section);
    }
    // LINE;
    printf("SECTION ptr:%p |len:%d | type:%d\n", section, section->len, section->sectype);
    printf("    keystr:|%s| id:|%d|\n", section->keystr, section->id);
    for (int ilen = 0; ilen < section->len; ilen++)
    {
        printf("%5d    ", ilen);
        print_p_toks_string(section->toks[ilen], true);
    }
    // print_memsegment(section->sector);
    // LINE;
}

void add_to_program(p_program_t program, p_memseg_t segment)
{
    if (program)
    {
        bool going = true;
        while (going)
        {
            if (program->n_used + (segment->memstop - segment->memstart) >= program->n_memorysize)
            {
                program->p_program = REALLOC_SAFE(program->p_program, program->n_memorysize + program->n_increment);
                // DPRINTF("TEST %d\n", program->n_memorysize);
                memset(program->p_program + program->n_memorysize, 0, program->n_increment);
                program->n_memorysize += program->n_increment;
            }
            else
            {
                going = false;
            }
        }
        for (int i = 0; i < segment->memstop - segment->memstart; i++)
        {

            program->p_program[program->n_used++] = segment->mem_bytes[i];
            // printf("used=%d byte=%x\n", program->n_used, segment->mem_bytes[i]);
        }
    }
}




p_section_t get_section_and_assign_to_idtable(p_context_t context, char *ref_key, section_type_t type)
{
    if(!ref_key)
    {
        DPRINTF("ref key empty%s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    const size_t keylength = strlen(ref_key);
    char *alloc_key = (char *)calloc(keylength + 1, sizeof(char));
    if (!alloc_key)
    {
        DPRINTF("MALLOC FAILED %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    memcpy(alloc_key, ref_key, keylength);
    p_section_t section = new_section(type, alloc_key);
    void *ref = make_reference(section);
    addto_hash_table(context->p_identifier_table, ref_key, ref);
    return section;
}

void stage1(p_context_t context, p_program_t program)
{
    const size_t max_sections = HASHTABLE_LEN;

    size_t lines = context->nlines, section_current_start = 0, sections_count = 0;
    bool iscurrent = false;
    char *key = NULL;

    p_tok_t *tokens = context->p_tokens;
    p_hashtable_t identifier_table = new_hash_table(HASHTABLE_LEN, free_reference);
    p_section_t *sections = (p_section_t *)calloc(max_sections, sizeof(p_section_t));
    context->p_identifier_table = identifier_table;

    size_t ntok_buffer_used = 0;
    size_t ntok_buffer_alloc = 1000;
    
    char *tok_buffer = (char *)calloc(ntok_buffer_alloc + 1, sizeof(char));

    for (size_t iter_line = 0; iter_line < lines; iter_line++)
    {
        // LINE;
        p_tok_t p_tok_line = tokens[iter_line];
        const size_t nstr_line = p_tok_line->nstr;
        if (p_tok_line->nstr == 0)
            continue;

        bool has_comment = false;
        size_t comment_split_at = nstr_line;
        for(size_t iter_p_sz_tok = 0; iter_p_sz_tok < p_tok_line->nstr; iter_p_sz_tok++)
        {
            
            int64_t pos = str_contains(p_tok_line->p_sz_toks[iter_p_sz_tok], ';');
            if(pos != -1)
            {
                has_comment = true;
                comment_split_at = iter_p_sz_tok;
                break;
            }
        }
        //DPRINTF("%d\n", comment_split_at);
        //print_p_toks_st(p_tok_line);
        //print_p_toks_string(p_tok_line, false);

        char *p_sz_first = p_tok_line->p_sz_toks[0];
        char lastchar = last_char(p_sz_first), firstchar = p_sz_first[0];
        const size_t ntok_first_len = strlen(p_sz_first);
        const size_t nafter_size = nstr_line - comment_split_at,
            nbefore_size = nstr_line - nafter_size;
        
        p_tok_t p_tok_before = cut_p_toks_st(p_tok_line, 0, nbefore_size), 
            p_tok_after = split_p_toks_st(p_tok_line, comment_split_at);
        //DPRINTF("%d\n", p_tok_line->nstr);
        //print_p_toks_string(p_tok_before, true);
        //DPRINTF("%d\n", p_tok_before->nstr);
        //print_p_toks_string(p_tok_after, true);
        //DPRINTF("%d\n", p_tok_after->nstr);
        if(p_tok_before->nstr == 0)
        {
            print_p_toks_string(p_tok_line, true);

            printf("Is comment\n");
            continue;
        }
        if (lastchar == ':')
        {

            const char torem[] = ":";
            iscurrent = true;
            section_current_start = iter_line;
            ntok_buffer_used = getstr_with_cut_chars(tok_buffer, ntok_buffer_alloc, p_sz_first, torem);
            //DPRINTF("function %s\n",tok_buffer);
            p_section_t current = get_section_and_assign_to_idtable(context, tok_buffer, function);

            if (cmpstrings(current->keystr, "START"))
            {
                current->id = 0;
                //print_section(current);
            }
            sections[sections_count++] = current;
            
        }
        else if (firstchar == '$')
        {
            
            const char torem[] = "$";
            iscurrent = true;
            section_current_start = iter_line;
            ntok_buffer_used = getstr_with_cut_chars(tok_buffer, ntok_buffer_alloc, p_sz_first, torem);
            //DPRINTF("constant %s\n", tok_buffer);
            p_section_t current = get_section_and_assign_to_idtable(context, tok_buffer, constant);
            sections[sections_count++] = current;
        }
        else
        {

            // DPRINTF("inside: %s \n", key);
            if (sections_count - 1 < 0 || sections_count - 1 >= max_sections)
            {
                DPRINTF("failed at %s:%d\n", __FILE__, __LINE__);
                exit(1);
            }
            //print_p_toks_st(p_tok_before);

            p_tok_t cutout = cut_substr_p_tok_t(p_tok_before, " ");
            print_p_toks_st(cutout);

            update_section(sections[sections_count - 1], cutout);
            free_p_toks_st(cutout);
            //print_section(sections[sections_count - 1]);
        }
        // print_p_toks_string(line);
        memset(tok_buffer, 0, ntok_buffer_alloc);
        free_p_toks_st(p_tok_after);
        free_p_toks_st(p_tok_before);
    }
    if (iscurrent)
    {
        // addto_hash_table(identifier_table, key, make_reference());
    }
    //print_hash_table(context->p_identifier_table);
    context->p_sections = sections;
    context->n_sections = sections_count; 
    stage2(context, program);

    for (int isec = 0; isec < sections_count; isec++)
    {
        p_section_t temp = sections[isec];
        free_section(sections[isec]);
    }

    free(sections);
}

void stage2(p_context_t context, p_program_t program)
{
    // LINE;
    // DPRINT("STAGE2\n");

    p_hashtable_t table = context->p_identifier_table;
    // print_hash_table(context->p_identifier_table);
    p_section_t *sections = context->p_sections;
    int len = context->n_sections;
    p_memseg_t *segments = (p_memseg_t *)calloc(len, sizeof(p_memseg_t));
    int bytecurrent = 3;
    int startsegment_iter = 0;

    for (int isec = 0; isec < len; isec++)
    {
        int stacklen = 0;
        int stackalloc = 1000;
        char *bytestack = (char *)calloc(stackalloc, sizeof(char));

        p_section_t temp = sections[isec];
        //print_section(temp);
        if (temp->id == 0)
        {
            startsegment_iter = isec;
        }

        // DPRINTF("___________%d__________\n", temp->id);
        // LINE;
        print_section(temp);

        for (int i = 0; i < temp->len; i++)
        {

            p_tok_t scroll = temp->toks[i];

            int nargs = 0, instruction = 0;
            if (stacklen >= stackalloc - 100)
            {
                // DPRINT("REALLOC\n");
                bytestack = REALLOC_SAFE(bytestack, stackalloc *= 2);
            }
            for (int iter_token = 0; iter_token < scroll->nstr; iter_token++)
            {
                char *p_sz_tok = scroll->p_sz_toks[iter_token];
                print_p_toks_st(scroll);
                DPRINTF("token->%s\n", p_sz_tok);

                if (temp->sectype == function)
                {
                    // DPRINT("function ");
                    if (iter_token == 0)
                    {

                        instruction = compare_with_instructions(p_sz_tok);

                        if (instruction > sizeof(str_instructions) / sizeof(*str_instructions) || instruction < 0)
                        {
                            DPRINTF("%s UNKNOWN INST\n", p_sz_tok);
                            exit(1);
                        }
                        nargs = len_instructions[instruction];
                        // DPRINTF("inst->%d nargs->%d\n", instruction, nargs);
                        bytestack[stacklen++] = instruction;
                        continue;
                    }
                    else
                    {
                        argument_type_t type = e_error;

                        void *data = getdata_from_hash_table(context->p_identifier_table, p_sz_tok);
                        if (data != NULL)
                        {
                            p_reference_t ref = get_reference(data);
                            //DPRINTF("REFERENCE %p\n", &ref);
                            type = e_ptr;
                            if (ref->sect->initalized == true)
                            {
                                //DPRINT("defined section \n");
                                unsigned int start = ref->sect->sector->memstart;

                                //DPRINTF("PTR = %d\n", start);

                                bytestack[stacklen++] = (unsigned char)start & 0xff;
                                bytestack[stacklen++] = (unsigned char)start >> 8;
                            }
                            else if (ref->sect == temp)
                            {
                                // DPRINT("self loop\n");
                                // DPRINTF("PTR = %d\n", bytecurrent);

                                bytestack[stacklen++] = (unsigned char)(bytecurrent & 0xff);
                                bytestack[stacklen++] = (unsigned char)(bytecurrent >> 8);
                            }
                            else
                            {
                                DPRINT("UNKNOWN FUNCTION\n");
                                exit(1);
                            }


                        }
                        else
                        {
                            type = get_argument_type(p_sz_tok);

                            print_argumenttype(type);

                            int size = decode_nbytes_for_argument(type, p_sz_tok);
                            char bytes[size];
                            // DPRINTF("argument size: %d\n", size);

                            size_t setcount = convert_token_to_bytes(p_sz_tok, type, bytes, size);
                            if (nargs != setcount)
                            {
                                LINE;
                                DPRINTF("TOO MANY ARGUMENTS %d %d\n", nargs, setcount);
                                DPRINTF("%s\n", p_sz_tok);
                                exit(1);
                            }

                            for (int byteiter = 0; byteiter < setcount; byteiter++)
                            {
                                // DPRINTF("0x%02x\n", (unsigned char)bytes[byteiter]);
                                bytestack[stacklen++] = (unsigned char)bytes[byteiter];
                            }
                        }
                    }
                }
                else if (temp->sectype == constant)
                {
                    DPRINT("constant ");
                    argument_type_t type = e_error;

                    type = get_argument_type(p_sz_tok);
                    print_argumenttype(type);

                    int size = decode_nbytes_for_argument(type, p_sz_tok);
                    char bytes[size];
                    // DPRINTF("argument size: %d\n", size);

                    size_t setcount = convert_token_to_bytes(p_sz_tok, type, bytes, size);
                    for (int byteiter = 0; byteiter < setcount; byteiter++)
                    {
                        // DPRINTF("0x%02x\n", (unsigned char)bytes[byteiter]);
                        bytestack[stacklen++] = (unsigned char)bytes[byteiter];
                    }
                }

                // line behavior
            }
        }
        // DPRINTF("%d %d\n", bytecurrent, stacklen);
        
        p_memseg_t segment = make_memsegment(bytestack, bytecurrent, bytecurrent + stacklen);
        // print_memsegment(segment);
        // DPRINTF("section initalized = true %p\n", temp);
        temp->initalized = true;
        temp->sector = segment;
        segments[isec] = segment;
        free(bytestack);
        bytecurrent += stacklen;
        // print_section(sections[isec]);
        // print_memsegment(sections[isec]->sector);
    }

    // print_memsegment(segments[startsegment_iter]);
    add_to_program(program, segments[startsegment_iter]);

    for (int i = 0; i < len; i++)
    {
        if (i == startsegment_iter)
            continue;
        add_to_program(program, segments[i]);

        // print_memsegment(segments[i]);
    }
    // printf("%d %d %d\n",program->n_increment, program->n_used, program->n_memorysize );
}

int assemble(const char *dir, char **bytes)
{

    p_tok_t *tokens = NULL;
    size_t lines = get_tokens(&tokens, dir);

    // exit(1);
    // DPRINTF("%lld\n", lines);
   
    size_t asmerr_ary_len = 1;
    const size_t increment = 1024;

    char *program_ptr = (char *)malloc(sizeof(char) * increment);
    memset(program_ptr, 0, increment);

    p_asmerr_t *asmerr_array = (p_asmerr_t *)calloc(asmerr_ary_len, sizeof(asmerr_t));

    program_t program = {.p_program = program_ptr, .n_memorysize = increment, .n_increment = increment, .n_used = 0};
    context_t context = {
        .nlines = lines,
        .p_identifier_table = NULL,
        .p_sz_fname = dir,
        .p_tokens = tokens,
        .p_sections = NULL,
        .n_sections = 0
    };
    stage1(&context, &program);
    asmerr_array[0] = NULL;
    //free_hash_table(context.p_identifier_table);
    // DPRINTF("%lld errors\n", errorcount);
    // DPRINT("End\n");

    *bytes = program.p_program;
   // LINE;
    //print_range(program.p_program, 0, 64, program.n_memorysize);
    return program.n_used;
}

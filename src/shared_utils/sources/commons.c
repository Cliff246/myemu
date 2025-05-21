#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hashmap.h"
#include "commons.h"

bool char_exist(char ref, char *p_sz_cmp)
{

    for(char *scroll = p_sz_cmp; *scroll != 0; scroll++)
    {
        if(*scroll == ref)
        {
            //printf("|%c| |%c| \n", ref, *scroll);

            return true;
        }
    }
    return false;
}



char last_char(char *ptr)
{
    char *last = ptr;
    for(; *(last + 1) != 0x00 ; last++);
    return *last;
}

void *__realloc_s_func(void *ptr, size_t size, const char *src_file, const char *call_function, size_t line_number)
{
    if(ptr == NULL)
    {
        DPRINTF("realloc failed at %s->%s->%lu\n%p -> %lu\n",src_file,call_function,line_number,ptr, size);

        fprintf(stderr, "realloc failed at %s->%s->%lu\n%p -> %lu\n",src_file,call_function,line_number,ptr, size);
        exit(1);
    }
    void *tptr = realloc(ptr, size);

    if(tptr == NULL)
    {
        DPRINTF("realloc failed at %s->%s->%lu\n%p -> %lu\n",src_file,call_function ,line_number ,ptr , size);
        fprintf(stderr, "realloc failed at %s->%s->%lu\n%p -> %lu\n",src_file, call_function, line_number, ptr, size);
        exit(1);
    }
    else
    {

        ptr = tptr;
        return ptr;
    }
}
void *__realloc_s_nofun(void *ptr, size_t size, const char *src_file,size_t line_number)
{
    if(ptr == NULL)
    {
        DPRINTF("realloc failed at %s->%lu\n%p -> %lu\n",src_file,line_number,ptr, size);
        #if DEBUG == 0
        fprintf(stderr, "realloc failed at %s->%lu\n%p -> %lu\n",src_file,line_number,ptr, size);
        #endif
        exit(1);
    }
    void *tptr = realloc(ptr, size);

    if(tptr == NULL)
    {
        DPRINTF("realloc failed at %s->%lu\n%p -> %lu\n",src_file,line_number,ptr, size);
        #if DEBUG == 0
        fprintf(stderr, "realloc failed at %s->%lu\n%p -> %lu\n",src_file,line_number,ptr, size);
        #endif
        exit(1);
    }
    else
    {
        #if REALLOC_SAFE_REPORTS == 1
        DPRINTF("realloc at %s->%lu\n", src_file, line_number);
        #endif
        ptr = tptr;
        return ptr;
    }
}

void print_bin(long bin, char len, bool newline)
{
#define BUFSIZE 65
    char buffer[BUFSIZE] = {0};
    for (int i = 0, b = len - 1; i < len; i++, b--)
    {
        char bit = GETBIT(bin, b);
        buffer[i] = (char)(bit + '0');
    }
    printf("%s", buffer);
    if(newline)
    {
        printf("\n");
    }
}
void print_hex(char hex, bool newline)
{
    printf("0x%02hhx%c", hex, (newline)? '\n':0x00);
}

void print_str_hex(char *str, bool newline)
{

    for(char *temp = str; *temp != 0; temp++)
    {
        print_hex(*temp, false);
        if(*temp == '\n')
            printf("->'%s' ", "\\n");
        else
            printf("->'%c' ", *temp);
    }
    if(newline)
        printf("\n");

}


//Maybe return new length
void reverse(char *ary)
{
    size_t len = strlen(ary);
    char tstr[len + 1];
    memset(tstr, 0, len + 1);
    strcpy(tstr, ary);
    int initial = 0, end = len - 1;
    for (int i = initial; i < end; i++)
    {
        char temp = tstr[i];
        tstr[i] = tstr[end];
        tstr[end] = temp;
        end--;
    }
    strcpy(ary, tstr);

}

//Maybe return new length
void trimr(char *pc)
{
    size_t len = strlen(pc);
    char strip[len + 1];
    memset(strip, 0, len + 1);
    reverse(pc);

    char *op = pc;
    size_t index = 0;
    bool stoptrim = false;
    while(*op++)
    {
        if(stoptrim == true)
        {
            strip[index++] = *op;
        }
        if((*op != ' ' && *op != '\n' && (unsigned char)*op != 0xff) && stoptrim == false)
        {
            strip[index++] = *op;
            stoptrim = true;
        }

    }
    reverse(strip);
    strcpy(pc, strip);
}


void triml(char *pc)
{
    size_t len = strlen(pc);
    char strip[len + 1];
    memset(strip, 0, len + 1);
    char *op = pc;
    size_t index = 0;
    bool stoptrim = false;

    while(*op)
    {
        if(stoptrim == true)
        {
            strip[index++] = *op;
        }
        if((*op != ' ' && *op != '\n') && stoptrim == false)
        {
            strip[index++] = *op;
            stoptrim = true;
        }
        op++;
    }
    strcpy(pc, strip);
}

bool cmpstrings(const char *str1, const char *str2)
{
    size_t l1 = strnlen(str1, 1000);
    size_t l2 = strnlen(str2, 1000);

    for (size_t i1 = 0; i1 < MAX(l1,l2); i1++)
    {
        if(i1 >= MIN(l1,l2))
            return false;

        char tmpc = str1[i1];
        if(tmpc != str2[i1])
            return false;
    }
    return true;
}



    



//this is cringe but if there is next in LinkedList then we will return it, if not well it will be null










size_t getstr_with_cut_chars(char *buffer, size_t buffer_size, const char *p_sz_ref, const char *p_sz_tocut)
{
    const size_t nref_len = strlen(p_sz_ref), ntocut_len = strlen(p_sz_tocut);
    size_t buffer_iter = 0;
    if (p_sz_ref && p_sz_tocut && buffer)
    {
        for (size_t iter_ref = 0; iter_ref < nref_len; iter_ref++)
        {
            if (buffer_iter >= buffer_size)
            {
                DPRINTF("buffer overflow %s:%d\n", __FILE__, __LINE__);
                exit(1);
            }

            char ref_cur = p_sz_ref[iter_ref];
            bool addtobuf = true;
            for (size_t iter_tocut = 0; iter_tocut < ntocut_len; iter_tocut++)
            {
                char tocut_cur = p_sz_tocut[iter_tocut];
                if (ref_cur == tocut_cur)
                {
                    addtobuf = false;
                    break;
                }
            }
            if (addtobuf)
            {
                buffer[buffer_iter++] = ref_cur;
            }
        }
        return buffer_iter;
    }
    else
    {
        DPRINTF("paramters are null %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
}

int64_t str_contains(const char *p_sz_ref, char check)
{
    size_t count = 0;
    for(char *c = (char *)p_sz_ref; *c != 0; ++c, ++count)
    {
        if(*c == check)
            return count;
    }
    return -1;
}

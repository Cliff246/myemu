#include "me_commons.h"

void *__realloc_s_func(void *ptr, size_t size, const char *src_file, const char *call_function, size_t line_number)
{
    if(ptr == NULL)
    {
        DPRINTF("realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
        
        fprintf(stderr, "realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
        exit(1);
    }
    void *tptr = realloc(ptr, size);
    
    if(tptr == NULL)
    {
        DPRINTF("realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
        fprintf(stderr, "realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
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
        DPRINTF("realloc failed at %s->%lld\n%p -> %lld\n",src_file,line_number,ptr, size);
        #if DEBUG == 0
        fprintf(stderr, "realloc failed at %s->%lld\n%p -> %lld\n",src_file,line_number,ptr, size);
        #endif
        exit(1);
    }
    void *tptr = realloc(ptr, size);
    
    if(tptr == NULL)
    {
        DPRINTF("realloc failed at %s->%lld\n%p -> %lld\n",src_file,line_number,ptr, size);
        #if DEBUG == 0
        fprintf(stderr, "realloc failed at %s->%lld\n%p -> %lld\n",src_file,line_number,ptr, size);
        #endif
        exit(1);
    }
    else
    {
        #if REALLOC_SAFE_REPORTS == 1
        DPRINTF("realloc at %s->%lld\n", src_file, line_number);
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
    errno_t err1, err2;
    size_t len = strlen(ary);
    char tstr[len + 1];
    memset(tstr, 0, len + 1);
    err1 = strcpy_s(tstr,len * sizeof(char) + 1 , ary);
    if(err1 != 0)
    {
        DPRINTF("HUGE STRCPY_S ERROR IN %s:%d", __FILE__, __LINE__);
        exit(1);
    }
    int initial = 0, end = len - 1;
    for (int i = initial; i < end; i++) 
    {
        char temp = tstr[i];
        tstr[i] = tstr[end];
        tstr[end] = temp;
        end--;
    }  
    err2 = strcpy_s(ary,len * sizeof(char) + 1 , tstr);
    if(err2 != 0)
    {
        DPRINTF("HUGE STRCPY_S ERROR IN %s:%d", __FILE__, __LINE__);
        exit(1);
    }

}

//Maybe return new length
void trimr(char *pc)
{
    errno_t err1;
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
        if((*op != ' ' && *op != '\n' && *op != 0xff) && stoptrim == false)
        {
            strip[index++] = *op;
            stoptrim = true;
        }

    }
    reverse(strip);
    err1 = strcpy_s(pc, len * sizeof(char) + 1, strip);
    if(err1 != 0)
    {
        DPRINTF("HUGE STRCPY_S ERROR IN %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}


void triml(char *pc)
{    
    errno_t err1;
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
    err1 = strcpy_s(pc, len * sizeof(char) + 1, strip);
    if(err1 != 0)
    {
        DPRINTF("HUGE STRCPY_S ERROR IN %s:%d", __FILE__, __LINE__);
        exit(1);
    }
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
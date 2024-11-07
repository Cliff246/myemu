#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "inc/me_myemu.h"


size_t hash_to_position(int64_t hash, size_t pos)
{
    size_t ret = hash % pos;
    return ret;
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
        DPRINTF("realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
        
        fprintf(stderr, "realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function,line_number,ptr, size);
        exit(1);
    }
    void *tptr = realloc(ptr, size);
    
    if(tptr == NULL)
    {
        DPRINTF("realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file,call_function ,line_number ,ptr , size);
        fprintf(stderr, "realloc failed at %s->%s->%lld\n%p -> %lld\n",src_file, call_function, line_number, ptr, size);
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
        if((*op != ' ' && *op != '\n' && *op != 0xff) && stoptrim == false)
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

p_hashtable_t new_hash_table(size_t size, free_hashtable_data_ptr freedata)
{
    p_hashtable_t table = (p_hashtable_t) calloc(1, sizeof(hashtable_t));
    if(table)
    {
        p_hashelem_t *list = (p_hashelem_t *)calloc(size, sizeof(p_hashelem_t));
        if(list)
        {
            //prob not needed but safer
            memset(list, 0, sizeof(p_hashelem_t) * size);
            table->tablesize = size;
            table->p_tablelist = list;
            table->freedata = freedata;
            return table;
        }
        else
        {
            DPRINTF("ALLOC ERROR %s:%d", __FILE__, __LINE__);
            exit(1);
        }
    }
    else
    {
        DPRINTF("ALLOC ERROR %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}

void free_hash_table(p_hashtable_t table)
{
    if(table)
    {
        p_hashelem_t *ary = table->p_tablelist;
        for(size_t iter = 0; iter < table->tablesize; iter++)
        {
            p_hashelem_t cur = ary[iter];
            //holy fuck this seems dangerous
            if(cur == NULL)
            {
                continue;
            }
            while(cur) 
            {
                cur = free_hash_element(cur, table->freedata);
            }
            if(cur)
                free_hash_element(cur, table->freedata);
        }
        free(table->p_tablelist);
        free(table);
    }

}

p_hashelem_t remove_from_hash_table(p_hashtable_t table, const char *key)
{
    if(table)
    {
        int64_t keyhash = hash(key);
        size_t position = hash_to_position(keyhash, table->tablesize);
        p_hashelem_t current = table->p_tablelist[position];
        if(current != NULL)
        {
            //in hashtable array
            if (cmpstrings(key, current->p_key))
            {

                if(current->p_next)
                {
                    p_hashelem_t next = current->p_next;
                    table->p_tablelist[position] = next;
                    current->p_next = NULL;
                    return current;
                }
                else
                {
                    table->p_tablelist[position] = NULL;
                    return current;
                }
            }
            //in hashtable linkedlist
            else
            {
                
                //loop through nexts

                for (p_hashelem_t prev = NULL; current; current = current->p_next)
                {
                    if (cmpstrings(key, current->p_key))
                    {
                        if (current->p_next != NULL)
                        {
                            prev->p_next = current->p_next;
                            current->p_next = NULL;
                            return current;
                        }
                        else
                        {
                            prev->p_next = NULL;
                            return current;
                        }
                    }
                    prev = current;
                }

                return NULL;
            }
        }
        //not in table
        else
        {
            return NULL;
        }
    }
    else
    {
        DPRINTF("TABLE NOT DEFINED %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}

p_hashelem_t new_hash_element(const char *str, void *data)
{
    if(str)
    {
        p_hashelem_t elem = (p_hashelem_t)malloc(sizeof(hashelem_t));
        if(elem)
        {
            //ew
            size_t len = strlen(str);
            char *copy = (char *)calloc(len + 1, sizeof(char));
            if(copy)
            {

                //i feel safe using strcpy cause really it should have broken earlier if it fails
                strcpy(copy, str);
                elem->p_data = data;
                elem->p_key = copy;
                elem->p_next = NULL;
                return elem;
            }
       
        }
     
    }
    //any fail
    {
        DPRINTF("FUNCTION FAIL  %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}
//this is cringe but if there is next in LinkedList then we will return it, if not well it will be null
p_hashelem_t free_hash_element(p_hashelem_t element, free_hashtable_data_ptr freedata)
{
    if(element)
    {
        p_hashelem_t next = element->p_next;
        free(element->p_key);
        freedata(element->p_data);
        free(element);
        return next;
    }
    else
    {
        DPRINTF("PARAMATER IS NULL: WILL FREE ANYWAYS %s:%d", __FILE__, __LINE__);
        free(element);
        return NULL;
    }
}

int64_t add_element_to_hashtable(p_hashtable_t table, p_hashelem_t elem)
{
    if(table && elem)
    {
        int64_t keyhash = hash(elem->p_key);
        size_t position = hash_to_position(keyhash, table->tablesize);
        p_hashelem_t current = table->p_tablelist[position];
        if(current == NULL)
        {
            table->p_tablelist[position] = elem;
        }
        else
        {
            if (cmpstrings(elem->p_key, current->p_key))
            {
                free_hash_element(elem, table->freedata);
                return -1;
            }
            for (; current->p_next != NULL; current = current->p_next)
            {
                if(cmpstrings(elem->p_key, current->p_key))
                {
                    free_hash_element(elem, table->freedata);
                    return -1;
                }
            }
            current->p_next = elem;
        }
        return position;
    }
    else
    {
        DPRINTF("PARAMATER IS NULL %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}

int64_t addto_hash_table(p_hashtable_t table, const char *key, void *data)
{
    if(table && key && data)
    {
        p_hashelem_t elem = new_hash_element(key, data);
        return add_element_to_hashtable(table, elem);
    }
    else
    {
        DPRINTF("PARAMATER IS NULL %s:%d", __FILE__, __LINE__);
        exit(1);
    }
}

p_hashelem_t get_from_hash_table(p_hashtable_t table, const char *key)
{
    if(table)
    {
        int64_t keyhash = hash(key);
        size_t position = hash_to_position(keyhash, table->tablesize);
        p_hashelem_t element = table->p_tablelist[position];
        if(element)
        {
            if(cmpstrings(element->p_key, key))
            {
                return element;
            }
            else
            {
                while(element)
                {
                    if(cmpstrings(element->p_key, key))
                    {
                        return element;
                    }
                    element = element->p_next;
                }
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        DPRINTF("PARAMATER IS NULL %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
}

void *getdata_from_hash_table(p_hashtable_t table, const char *key)
{
    p_hashelem_t elem = get_from_hash_table(table, key);
    if(elem)
    {
        //printf("%s\n", elem->p_key);
        return elem->p_data;
    }
    else
    {
        //DPRINTF("key not in table%s:%d\n", __FILE__, __LINE__);
        //exit(1);
        return NULL;
    }
}

int64_t hash(const char *key)
{
    int64_t result = 0x0123456789abcdef;
    while (*key)
    {
        result ^= *(key++);
        result = result << 5;
    }
    return result;
}

void print_hash_elem(p_hashelem_t elem)
{
    printf("element: data:%p key:%s next:%p\n", elem->p_data, elem->p_key, elem->p_next);
}

void print_hash_table(p_hashtable_t table)
{   
    if(table)
    {
        for(int i = 0; i < table->tablesize; i++)
        {

            if(table->p_tablelist[i] != NULL)
            {
                p_hashelem_t cur = table->p_tablelist[i];
                for(;cur != NULL; cur = cur->p_next)
                {
                    print_hash_elem(cur);
                }
            }

        }
    }


}

void print_range(char *range, size_t start, size_t stop, size_t size)
{
    if (size < stop)
    {
        return;
    }
    int instruction = range[start];
    int arglen = 0;
    for (size_t i = start; i < stop; i++)
    {
        print_bin(range[i], 8, 0);
        printf(" %02lld  %04d ", i, range[i]);

        print_hex(range[i], 0);
        printf(" ");
        if (isprint(range[i]))
        {
            printf("|%c| ", range[i]);
        }
        else
        {
            printf("____");
        }

        if (range[i] >= 0 && range[i] < sizeof(str_instructions) / sizeof(*str_instructions))
        {
            printf(" %s", str_instructions[(size_t)range[(int)i]]);
            instruction = range[i];
        }
        printf("\n");
    }
}
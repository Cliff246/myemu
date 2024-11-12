#ifndef __ME_COMMONS__
#define __ME_COMMONS__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#include "me_myemu.h"



#define DEBUG 1
#define REALLOC_SAFE_REPORTS 0
#define HASHTABLE_LENGTH 10000


#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))

#define BIT(n) (0x01 << n)
#define SETBYTE(n, v, k) (unsigned char)(((unsigned short)n & ~(0xFF << ((k) * 8))) | ((v & 0xFF) << ((k) * 8)))
#define GETBYTE(n, k) (unsigned char)(((unsigned short)n >> ((k) * 8)) & 0xFF)

#define COMBINE(A, B) ( ((unsigned short)A) | ( ((unsigned short)B) << (unsigned short)0x08) )
#define SPLITL(A) (char)((unsigned short)A & 0x00ff)
#define SPLITR(B) (char)((unsigned short)B & 0xff00)
#define STR(x) #x
#define XSTR(x) STR(x)
#define CONCAT(a, b) a##b

#define ARY_LENGTH(ary) (sizeof(ary) / sizeof(ary[0]))
#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#if DEBUG == 1
#define OFFSET_DPRINTF(f, ...) \
    DPRINTF("%s", offset);     \
    DPRINTF(f, __VA_ARGS__)    
#define DPRINTF(f, ...) printf(f, __VA_ARGS__)
#define DPRINT(f) printf(f)
#define LINE printf("\n------%s:%d-------\n", __FILE__, __LINE__)
#else
#define OFFSET_DPRINTF(f, ...) printf()
#define DPRINT(f)
#define DPRINTF(f, ...)
#define LINE
#endif

typedef char *pstr_t;
typedef void (*free_hashtable_data_ptr)(void *);

typedef struct hash_element
{
    char *p_key;
    void *p_data;
    struct hash_element *p_next;
}hashelem_t, *p_hashelem_t;

typedef struct hash_table
{
    p_hashelem_t *p_tablelist;
    size_t tablesize;
    free_hashtable_data_ptr freedata;
}hashtable_t, *p_hashtable_t;

void *__realloc_s_func(void *ptr, size_t size, const char *src_file, const char *call_function, size_t line_number);
void *__realloc_s_nofun(void *ptr, size_t size, const char *src_file, size_t line_number);

#ifdef __func__
#define REALLOC_SAFE(ptr, size) __realloc_s_func(ptr, size, __FILE__, __func__, __LINE__)
#else
#define REALLOC_SAFE(ptr, size) __realloc_s_nofun(ptr, size, __FILE__, __LINE__)
#endif

void print_bin(long bin, char len, bool newline);
void print_hex(char hex, bool newline);
void print_str_hex(char *str, bool newline);
void print_range(char *range, size_t start, size_t stop, size_t size);

int64_t hash(const char *key);
p_hashtable_t new_hash_table(size_t size, free_hashtable_data_ptr freedata);
void free_hash_table(p_hashtable_t table);

int64_t addto_hash_table(p_hashtable_t table, const char *key, void *data);
int64_t add_element_to_hashtable(p_hashtable_t table, p_hashelem_t elem);
// copyies key string so memory overhead so whoop whoop, but also saftey
// also i am lazy
p_hashelem_t new_hash_element(const char *str, void *data);
// cringe cause this can have a next ptr and we would lose a bunch of memory by just getting rid of it
// so i will give it back if it exists
p_hashelem_t free_hash_element(p_hashelem_t element, free_hashtable_data_ptr freedata);

p_hashelem_t get_from_hash_table(p_hashtable_t table, const char *key);
p_hashelem_t remove_from_hash_table(p_hashtable_t table, const char *key);
void *getdata_from_hash_table(p_hashtable_t table, const char *key);

void print_hash_table(p_hashtable_t table);
void print_hash_elem(p_hashelem_t elem);

void reverse(char *ary);
void triml(char *pc);
void trimr(char *pc);
char last_char(char *ptr);
size_t getstr_with_cut_chars(char *buffer, size_t buffer_size, const char *p_sz_ref, const char *p_sz_tocut);
int64_t str_contains(const char *p_sz_ref, char check);
bool char_exist(char ref, char *p_sz_cmp);

bool cmpstrings(const char *str1, const char *str2);

#endif

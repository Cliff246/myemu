#ifndef __COMMONS__
#define __COMMONS__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#define DEBUG 1
#define REALLOC_SAFE_REPORTS 1



#define SETBIT(n, k) (1 | (n << k))
#define CLRBIT(n, k) (n & ~(1 << k))
#define GETBIT(n, k) (1 & (n >> k))

#define BIT(n) (0x01 << n)

#define COMBINE(a, b) (ushort)(((ushort)a) | ((ushort)b) << 8)
#define SPLITL(a) (char)((ushort)a & 0x00ff)
#define SPLITR(b) (char)((ushort)b & 0xff00)
#define STR(x)  #x
#define XSTR(x) STR(x)
#define CONCAT(a,b) a ## b

#define ARY_LENGTH(ary) (sizeof(ary)/sizeof(ary[0]))
#define MAX(a,b) ((a > b)? a:b)
#define MIN(a,b) ((a < b)? a:b)



#if DEBUG == 1 
#define OFFSET_DPRINTF(f, ...) DPRINTF("%s", offset); DPRINTF(f, __VA_ARGS__)
#define DPRINTF(f,...) printf(f,__VA_ARGS__)
#define DPRINT(f)  printf(f) 
#define LINE printf("\n------%s:%d-------\n",__FILE__,__LINE__)
#else
#define OFFSET_DPRINTF(f, ...) printf()
#define DPRINT(f)  
#define DPRINTF(f,...)
#define LINE()
#endif


typedef unsigned short ushort;


void *__realloc_s_func(void *ptr, size_t size, const char *src_file, const char *call_function, size_t line_number);
void *__realloc_s_nofun(void *ptr, size_t size, const char *src_file, size_t line_number);
#ifdef __func__
#define REALLOC_SAFE(ptr, size) __realloc_s_func(ptr, size, __FILE__, __func__,__LINE__)
#else
#define REALLOC_SAFE(ptr, size) __realloc_s_nofun(ptr, size, __FILE__, __LINE__)
#endif
void print_bin(long bin, char len, bool newline);
void print_hex(char hex, bool newline);

void print_str_hex(char *str, bool newline);
void reverse(char *ary);
void triml(char *pc);
void trimr(char *pc);

bool cmpstrings(const char *str1,const char *str2);

#endif
#ifndef __MODULES_COMMONS__
#define __MODULES_COMMONS__


#ifdef _WIN32
#  define MODULES_EXPORT __declspec(dllexport)
#else
#  define MODULES_EXPORT __attribute__((visibility("default")))
#endif

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "dlfcn.h"


#endif

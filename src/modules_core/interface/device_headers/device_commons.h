#ifndef __DEVICE_COMMONS__
#define __DEVICE_COMMONS__

#ifdef _WIN32
#  define DEVICE_EXPORT __declspec(dllexport)
#else
#  define DEVICE_EXPORT __attribute__((visibility("default")))
#endif


#include <stdbool.h>
#include <stdio.h>

#endif

#ifndef __MODULES__
#define __MODULES__

#include <dlfcn.h>
#include "device_descriptor.h"



typedef struct module 
{
	char *path;	
	int flags;
	void *handle;
}module_t, *p_module_t;

p_module_t open_module(const char *path);
void close_module(p_module_t module);

p_devdesc_t get_device_from_module(p_module_t module);

#endif

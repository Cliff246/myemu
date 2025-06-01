#ifndef __DEV__
#define __DEV__

#include "commons.h"
#include "shared_ptr.h"

typedef struct device device_t, *p_device_t; 

typedef struct device
{
	
	p_shared_t device_descriptor_shared;
}device_t, *p_device_t;



#endif

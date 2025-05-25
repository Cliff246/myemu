#ifndef __MODULES__
#define __MODULES__

#include <stdlib.h>
#include "modules_commons.h"
#include "device_descriptor.h"
#include "device.h"


typedef struct modtop
{
	const char *module_name;

	void (*initialize)(void);
	void (*shutdown)(void);

	p_devdesc_t (*get_device_description)(void);
	
	size_t device_count;
	const p_device_t *(*list_devices)(void);

}modtop_t, *p_modtop_t;




MODULES_EXPORT const p_modtop_t get_modtop(void);

#endif




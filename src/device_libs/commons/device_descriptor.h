#ifndef __DEVICE_DESCRIPTOR__
#define __DEVICE_DESCRIPTOR__

#include "device_commons.h"
#include "device_interface.h"
#include "device_files.h"
#include "device_assembler_core.h"
#include "device_emulator_core.h"
#define FLAG_LENGTH 10

typedef struct device_descriptor
{
	//lib identifiers
	int id;
	const char *device_name;
	const char *device_typeclass;
	const char *lib_name;
	//lib features
	bool flags[FLAG_LENGTH];
	bool carries_emulation_core;
	bool carries_assembly_core;

	void (*create_device)(void);
	void (*free_device)(void);	
	
		
	p_devinfc_t device_interface;

	p_devasm_t device_assembler_core;
	p_devemu_t device_emulator_core;
	

	p_devfilesys_t device_file_system;


	//contains extras
	void *extras;

}devdesc_t, *p_devdesc_t;


#endif


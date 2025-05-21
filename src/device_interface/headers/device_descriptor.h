#ifndef __DEVICE_DESCRIPTOR__
#define __DEVICE_DESCRIPTOR__

#ifdef _WIN32
#  define DEVICE_EXPORT __declspec(dllexport)
#else
#  define DEVICE_EXPORT __attribute__((visibility("default")))
#endif


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
	int version;
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
	



	//contains extras
	void *extras;

}devdesc_t, *p_devdesc_t;

DEVICE_EXPORT const p_devdesc_t get_device_description();

#endif


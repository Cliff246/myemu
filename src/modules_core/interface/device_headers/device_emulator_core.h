#ifndef __DEVICE_EMULATOR_CORE__
#define __DEVICE_EMULATOR_CORE__



typedef void (*device_tick)(void *dev);

typedef struct device_emulator
{
	device_tick tick;			
}devemu_t, *p_devemu_t;

#endif

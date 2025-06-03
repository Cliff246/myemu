#ifndef __RISCV_DEVICE_EMULATOR_CORE__
#define __RISCV_DEVICE_EMULATOR_CORE__

#include "commons.h"




typedef struct riscv_emu
{
	int id;
	int fields;
	uint8_t *buffer_inp;
	uint8_t *buffer_out;

	void *message_queue;

	void (*message_handler)(struct riscv_emu *);


	void *extras;
		
}riscv_emu_t, *p_riscv_emu_t;

#endif

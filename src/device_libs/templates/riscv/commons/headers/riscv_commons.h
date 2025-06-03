#ifndef __RISKV_COMMONS__
#define __RISKV_COMMONS__

#include "device_descriptor.h"

#include "stdint.h"



typedef struct riscv_cpu
{
	int flags;

	size_t extentions_count;
	void *extentions;		
	
		


}riscv_cpu_t, *p_riscv_cpu_t;

extern p_devdesc_t desc;

extern uint32_t riscv_inst_t; 





#endif

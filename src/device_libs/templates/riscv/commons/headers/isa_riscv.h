#ifndef __ISA_RISC_V__
#define __ISA_RISC_V__

#include "commons.h"
#include "tokenizer.h"

typedef enum riscv_core_instruction_format
{
	FMT_R,
	FMT_I,
	FMT_S,
	FMT_SB,
	FMT_U,
	FMT_UJ
}riscv_core_instfmt_t;

typedef struct riscv_inst_class
{
	int standard;
	riscv_core_instfmt_t fmt;
	const char *(*get_description)(void);
	const char *(*get_details)(void);
	const char *(*get_name)(void);
	const char *(*get_fmt_class)(void);
	const char *(*get_mnemonic)(void);
	size_t (*get_byte_length)(void);
	
	size_t (*get_number_of_paramaters)(void);
	char *(*paramater_describe)(int start, int stop);

	uint64_t (*convert_to_binary)(const char *assembly);

}riscv_inst_class_t;





#endif

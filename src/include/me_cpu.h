#ifndef __ME_CPU__
#define __ME_CPU__

#include "me_myemu.h"
#include "me_commons.h"


#define CYCLES 1000

// two bytes for each instruction
#define INSTRUCTION_BYTESIZE 2
// mv add sub syscall
#define ASMINST_MNMONIC_STR_SIZE 10
// paramters define what the instruction affect and take in for type and registers
//  mnmonic param0, param1, param2, ... , param8, param9 arg0 arg2 arg3 arg4
// for immedate values #byte, #word, #dword, #qword, #ptr, #float, #double, #byte'L #byte'H splits args into low and high
// for single register %a %b %c %d %pc %sp %b %dl %dh
// for offset [%a + %b + num2 * num2]
// expected paramaters and what they do are decided by the mnmonic
// its supposed to go mnmonic {params} args
// for inst ADD_ABC:     add %a,%b,%c
// for INST_ADD_ABA:     add %a,%b,%a
// for inst ADD_AB_D:    add %a,%b,%d
// for inst ADD_AI_C:    add %a, #byte, %c $num
// for inst ADD_AI_A:    add %a, #byte, %a $num
// for inst MV_A_DL:     move %a, %dl
// for inst SET_A_I:     move %a, #byte $0xff
// for inst PUSH_A :     push %a
// for inst PUSH_B :     push %b
// for inst PUSH_AB:     push %a, %b
// for INST PUSH_I:      push #byte $0x10
// for inst CALLF_I:     call #ptr @ref
// for inst CALLF_D:     call %d
// for inst CALLFAB:     call %a, %b
// for inst JMP_LE_I:    jump&lt #ptr @ref
// for inst JMP_AB:      jump %a,%b
// for inst JMP_AI:      jump %a, #ptr !@referene
#define ASMINST_MAX_PARAMATERS 10
#define ASMINST_MAX_ARGUMENTS 10

typedef enum {
    _endian_litte, 
    _endian_big,
    _endian_undefined,
}endian_type_t;

#define ASMINST_MNMONIC_STR_SIZE 10

#define REGISTER_IDENTIFIER_SIZE 4

typedef struct instruction
{
    char instref[10];
    int nargs;
    int param_reg[ASMINST_MAX_PARAMATERS];
    int result_dest;

} vcpuinst_t, *p_vcpuinst_t;

typedef struct virtual_register
{
    char *vreg_str;
    int vreg_regid;

    int vreg_ncontent;
    char *vreg_content;
    
        
}vreg_t, *p_vreg_t;

typedef struct procesor_descriptor
{
    char *file_srcdir;

}vcpu_desc_t, *p_vcpu_desc_t;

typedef struct virtual_object
{
    int id;
    char *desc;
}vobj_t, *p_vobj_t;

typedef struct processor_emulator
{
    
}vcpu_emu_t, p_vcpu_emu_t;

typedef struct virtual_processors_debug
{

}vcpu_debug_t, *p_vcpu_debug_t;


typedef struct virtual_procesor
{
    vcpu_desc_t vcpu_description;
    vcpu_emu_t vcpu_emulator;

    int vcpu_nregisters;

    vreg_t *vcpu_registers;

    int vcpu_memory_size;
    char *vcpu_memory;

    int vcpu_address_bus_size;
    char *vcpu_address_bus;


    int vcpu_data_bus_size;
    char *vcpu_data_bus;

    int vcpu_instructions_size;
    vcpuinst_t *vcpu_instructions;
    

}vproc_t, *p_vproc_t;

typedef struct virtual_systree
{
    int n_in_nodes;
    p_vobj_t *in_nodes;
    int n_out_nodes;
    p_vobj_t *out_nodes;
}vsystree_t, *p_vsystree_t;

typedef struct virtual_computer
{
    vproc_t vcom_processor;

} vcomputer_t, *p_vcomputer_t;

void emulate(char *program, size_t size, int cycles);

#endif
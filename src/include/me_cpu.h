#ifndef __ME_CPU__
#define __ME_CPU__

#include "me_myemu.h"
#include "me_commons.h"

#define CYCLES 1000

typedef struct byteset
{
    unsigned char num;
    char bytes[];
}byteset_t, *p_byteset_t;

typedef void(*set_register)(p_byteset_t);

#define REGISTER_IDENTIFIER_SIZE 4
typedef enum vcpu_register_type
{
    single, 
    multiple,
}vreg_type_t;

typedef struct vcpu_register
{

    char vreg_identifier[REGISTER_IDENTIFIER_SIZE];
    vreg_type_t vreg_type;

}vreg_t, *p_vreg_t;

typedef struct instruction_set
{
    size_t size;
}instset_t, *p_instset_t;

typedef struct virtual_procesor
{
    size_t vcpu_mem_size;
    size_t vcpu_mem_width;
    size_t vcpu_mem_alloc;
    char *vcpu_memory_ptr;
    p_instset_t vcpu_instructions_set;     
    size_t vcpu_register_count;

    p_byteset_t *vcpu_registers;
    
}vproc_t, *p_vproc_t;

#define FOREACH_INST(INST)        \
    INST(NOTHING_, 0)             \
    /*Memory access and manip*/   \
    INST(M_GET_AB, 0)             \
    INST(M_GET_II, 2)             \
    INST(M_GET_D_, 0)             \
    INST(M_SET_A_, 3)             \
    /*Adding with carry */        \
    INST(ADDC_ABC, 0)             \
    INST(ADDC_AIC, 1)             \
    INST(ADDC_IIC, 2)             \
    INST(ADD_AB_D, 0)             \
    /* Subtracting with carry*/   \
    INST(SUBC_ABC, 0)             \
    INST(SUBC_AIC, 1)             \
    INST(SUBC_IIC, 2)             \
    INST(SUB_AB_D, 0)             \
    /*Set registers immedatly*/   \
    INST(SET_I__A, 1)             \
    INST(SET_I__B, 1)             \
    INST(SET_I_AB, 2)             \
    INST(SET_I__D, 2)             \
    INST(SET_I_DH, 1)             \
    INST(SET_I_DL, 1)             \
    /*move around register data*/ \
    INST(MV_B_A__, 0)             \
    INST(MV_A_B__, 0)             \
    INST(MV_C_A__, 0)             \
    INST(MV_C_B__, 0)             \
    INST(MV_B_C__, 0)             \
    INST(MV_A_C__, 0)             \
    INST(MV_X_Y__, 0)             \
    INST(MV_Y_X__, 0)             \
    INST(MV_A_X__, 0)             \
    INST(MV_X_A__, 0)             \
    /*D register moves  */        \
    INST(MV_DL_A_, 0)             \
    INST(MV_DH_A_, 0)             \
    INST(MV_DL_B_, 0)             \
    INST(MV_DH_B_, 0)             \
    INST(MV_DL_C_, 0)             \
    INST(MV_DH_C_, 0)             \
    INST(MV_A_DL_, 0)             \
    INST(MV_A_DH_, 0)             \
    INST(MV_B_DL_, 0)             \
    INST(MV_B_DH_, 0)             \
    INST(MV_C_DL_, 0)             \
    INST(MV_C_DH_, 0)             \
    INST(MV_AB_D_, 0)             \
    INST(MV_XY_D_, 0)             \
    INST(MV_D_AB_, 0)             \
    INST(MV_D_XY_, 0)             \
    INST(MV_DL_X_, 0)             \
    INST(MV_DH_Y_, 0)             \
    /*Jmp with no conditions*/    \
    INST(JMP____I, 2)             \
    INST(JMP___AB, 0)             \
    INST(JMP_PC_I, 1)             \
    INST(JMP____D, 0)             \
                                  \
    /*Compare Between data*/      \
    INST(CMP__A_B, 0)             \
    INST(CMP__I_A, 1)             \
    INST(CMP__I_I, 2)             \
    /*JMP WITH Condition*/        \
    INST(JMP_NE_I, 2)             \
    INST(JMP_EQ_I, 2)             \
    INST(JMP_LT_I, 2)             \
    INST(JMP_GT_I, 2)             \
    /*INC_A and DEC_B*/           \
    INST(INC_A___, 0)             \
    INST(DEC_A___, 0)             \
    INST(INC_X___, 0)             \
    INST(INC_Y___, 0)             \
    INST(DEC_X___, 0)             \
    INST(DEC_Y___, 0)             \
    /*Stack Stuff*/               \
    INST(PUSH_I__, 1)             \
    INST(PUSH_A__, 0)             \
    INST(POP_A___, 0)             \
    INST(PEAK_A__, 0)             \
    INST(PUSH_RP_, 0)             \
    INST(POP_RP__, 0)             \
    /*Stop Computer*/             \
    INST(__STOP__, 0)             \
    /*Function implementation*/   \
    INST(CALLF_II, 2)             \
    INST(CALLF_AB, 0)             \
    INST(CALLF_D_, 0)             \
    /*Return from call*/          \
    INST(RETURN__, 0)             \
    INST(OUTPUT_A, 0)             \
    INST(PRINTC_A, 0)             \
    INST(OUTPUT_D, 0)             \

//cycles = -1 means forever
void emulate(char *program, size_t size, int cycles);

#define GENERATE_INSTRUCTION_ENUM(ENUM, LEN) ENUM,
#define GENERATE_INSTRUCTION_STRING(STRING, LEN) #STRING,
#define GENERATE_INSTRUCTION_LEN(STRING, LEN) LEN,


typedef enum enum_instructions
{
    FOREACH_INST(GENERATE_INSTRUCTION_ENUM)
} op;

static const char *str_instructions[] = {
    FOREACH_INST(GENERATE_INSTRUCTION_STRING)};
static const int len_instructions[] = {
    FOREACH_INST(GENERATE_INSTRUCTION_LEN)};

#endif
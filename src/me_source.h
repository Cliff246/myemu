#ifndef __ME_SOURCE__
#define __ME_SOURCE__

#define FOREACH_INST(INST)         \
    INST(NOTHING_, 0)              \
/*Memory access and manip*/        \
    INST(M_GET_AB, 2)              \
    INST(M_SET_A_, 2)              \
/*Adding with carry */             \
    INST(ADDC_ABC, 0)              \
    INST(ADDC_AIC, 1)              \
    INST(ADDC_IIC, 2)              \
/* Subtracting with carry*/        \
    INST(SUBC_ABC, 0)              \
    INST(SUBC_AIC, 1)              \
    INST(SUBC_IIC, 2)              \
/*Set registers immedatly*/        \
    INST(SET_I__A, 1)              \
    INST(SET_I__B, 1)              \
/*move around register data*/      \
    INST(MOVE_B_A, 0)              \
    INST(MOVE_A_B, 0)              \
    INST(MOVE_C_A, 0)              \
    INST(MOVE_C_B, 0)              \
    INST(MOVE_B_C, 0)              \
    INST(MOVE_A_C, 0)              \
/*Jmp with no conditions*/         \
    INST(JMP____I, 2)              \
    INST(JMP__A_B, 2)              \
/*Compare Between data*/           \
    INST(CMP__A_B, 0)              \
    INST(CMP__I_A, 1)              \
    INST(CMP__I_I, 2)              \
/*JMP WITH Condition*/             \
    INST(JMP_NE_A, 2)              \
    INST(JMP_EQ_A, 2)              \
    INST(JMP_LT_A, 2)              \
    INST(JMP_GT_A, 2)              \
/*INC_A and DEC_B*/                \
    INST(INC_A___, 0)              \
    INST(DEC_A___, 0)              \
/*Stack Stuff*/                    \
    INST(PUSH_I__, 1)              \
    INST(PUSH_A__, 1)              \
    INST(POP_A___, 0)              \
/*Stop Computer*/                  \
    INST(__STOP__, 0)              \
/*Function implementation*/        \
    INST(CALLF_II, 2)              \
    INST(CALLF_AB, 0)              \
/*Return from call*/               \
    INST(RETURN__, 0)              \







#define GENERATE_INSTRUCTION_ENUM(ENUM, LEN) ENUM, 
#define GENERATE_INSTRUCTION_STRING(STRING, LEN) #STRING,
#define GENERATE_INSTRUCTION_LEN(STRING, LEN) LEN,
typedef enum enum_instructions
{
    FOREACH_INST(GENERATE_INSTRUCTION_ENUM)
}op;

static const char *str_instructions[] = {
    FOREACH_INST(GENERATE_INSTRUCTION_STRING)
};
static const int len_instructions[] = {
    FOREACH_INST(GENERATE_INSTRUCTION_LEN)
};





#endif
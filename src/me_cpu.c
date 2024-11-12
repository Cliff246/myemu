#include "me_myemu.h"
#include "me_cpu.h"
#include "me_instruction.h"
#include "me_commons.h"

void emulate(char *program, size_t size, int cycles)
{
#define GET_CONDITON_FLAG ((char)reg_flg & 3)
#define SET_CONDITON_FLAG(x) (reg_flg += ((char)x & 3))
#define A_NOT_EQUAL__B 0
#define A_EQLUAL__TO_B 1
#define A_IS_GREATER_B 2
#define A_IS_SMALLER_B 3
#define NEXT (memory[++pc])
#define UPDATENEXT DPRINTF("item %u at address %d\n", memory[pc], pc)
#define EMULATOR_RAM 1024
    unsigned char memory[EMULATOR_RAM];
    unsigned short pc = 0, sp = 1024 - 100, rp = 0;
    unsigned short reg_D = 0;
    unsigned char reg_A = 0, reg_B = 0, reg_C = 0, reg_X = 0, reg_Y = 0, reg_flg = 0;
    memset(memory, 0, EMULATOR_RAM * sizeof(*memory));
    memcpy(memory, program, sizeof(program) * size);

    print_range(memory, 0, 80, 1024);
    char inst[CYCLES];
    memset(inst, 0, CYCLES);
    bool con = true;
    size_t i = 0;
    while(con)
    {

        if(i >= cycles && cycles != -1)
        {
            break;
        }
        unsigned char args[3];
        unsigned short startpc = pc;
        int opcode = startpc[memory],
            oplen = len_instructions[opcode];
        // DPRINTF("%d %d\n", opcode, oplen);
#ifdef DEBUG
        DPRINTF("pc ->%d sp->%d\n", pc, sp);
        DPRINTF("A= %d, B= %d, C= %d X=%d Y=%d\n", reg_A, reg_B, reg_C, reg_X, reg_Y);
        DPRINTF("D= %u, DL = %u, DH = %u\n", reg_D, GETBYTE(reg_D, 0), GETBYTE(reg_D, 1));
        // printrange(memory, 1024 - 100, 1024-90, 1024);

        DPRINTF("opcode ->%s oplen ->%d\n\n", str_instructions[opcode], oplen);
        // print_bin(reg_flg, 8, 1);
        if (oplen == 2)
        {
            DPRINTF("ARG COMBINED = %d\n\n", (unsigned short)COMBINE(args[0], args[1]));
        }
        if (oplen >= 3)
        {
            DPRINT("break\n\n");
            break;
        }
#endif

       
        for (int i = 0; i < oplen; i++)
        {
            args[i] = NEXT;
            UPDATENEXT;
        }
        

        inst[i] = opcode;
        switch (opcode)
        {

        // MEMSET
        case M_SET_A_:

            memory[COMBINE(args[0], args[1])] = reg_A;
            break;
        // MEMGET
        case M_GET_AB:

            reg_A = memory[COMBINE(reg_A, reg_B)];
            break;
        case M_GET_II:
            reg_A = memory[COMBINE(args[0], args[1])];
            break;
        case M_GET_D_:
            reg_A = memory[reg_D];
            break;
        // ADD WITH CARRY
        case ADDC_ABC:
            reg_C = reg_B + reg_A;

            break;
        case ADDC_AIC:
            reg_C = args[0] + reg_A;
            break;
        case ADDC_IIC:

            reg_C = args[0] + args[1];
            break;
        case ADD_AB_D:
            reg_D = reg_A + reg_B;
            break;
        // SUB WITH CARRY
        case SUBC_AIC:
            reg_C = reg_A - args[0];
            break;
        case SUBC_IIC:
            reg_C = args[0] - args[1];
            break;
        case SUBC_ABC:
            reg_C = reg_A - reg_B;

            break;
        case SUB_AB_D:
            reg_D = reg_A - reg_B;
            break;

            // SET REGISTER USING IMEDIATE

        case SET_I_AB:
            reg_A = args[0];
            reg_B = args[1];
            break;

        case SET_I__A:
            reg_A = args[0];
            break;
        case SET_I__B:
            reg_B = args[0];
            break;
        case SET_I__D:
            reg_D = COMBINE(args[0], args[1]);
            break;
        case SET_I_DH:
            reg_D = SETBYTE(reg_D, args[0], 1);
            break;
        case SET_I_DL:
            reg_D = SETBYTE(reg_D, args[0], 0);
            break;

        // SWAP REGISTERS
        case MV_A_B__:
            reg_B = reg_A;
            break;
        case MV_B_A__:
            reg_A = reg_B;
            break;
        case MV_B_C__:
            reg_C = reg_B;
            break;
        case MV_A_C__:
            reg_C = reg_A;
            break;
        case MV_C_A__:
            reg_A = reg_C;
            break;
        case MV_C_B__:
            reg_B = reg_C;
            break;
        case MV_X_Y__:
            reg_Y = reg_X;
            break;
        case MV_Y_X__:
            reg_X = reg_Y;
            break;
        case MV_A_X__:
            reg_X = reg_A;
            break;
        case MV_X_A__:
            reg_A = reg_X;
            break;
        case MV_DL_A_:
            reg_A = GETBYTE(reg_D, 0);
            break;
        case MV_DH_A_:
            reg_A = GETBYTE(reg_D, 1);
            break;
        case MV_DL_B_:
            reg_B = GETBYTE(reg_D, 0);
            break;
        case MV_DH_B_:
            reg_B = GETBYTE(reg_D, 1);
            break;
        case MV_DL_C_:
            reg_C = GETBYTE(reg_D, 0);
            break;
        case MV_DH_C_:
            reg_C = GETBYTE(reg_D, 1);
            break;
        case MV_A_DL_:
            reg_D = SETBYTE(reg_D, reg_A, 0);
            break;
        case MV_A_DH_:
            reg_D = SETBYTE(reg_D, reg_A, 1);
            break;
        case MV_B_DL_:
            reg_D = SETBYTE(reg_D, reg_B, 0);
            break;
        case MV_B_DH_:
            reg_D = SETBYTE(reg_D, reg_B, 1);
            break;
        case MV_C_DL_:
            reg_D = SETBYTE(reg_D, reg_C, 0);
            break;
        case MV_C_DH_:
            reg_D = SETBYTE(reg_D, reg_C, 1);
            break;
        case MV_AB_D_:
            reg_D = SETBYTE(reg_D, reg_A, 0) + SETBYTE(reg_D, reg_B, 1);
            break;
        case MV_XY_D_:
            reg_D = SETBYTE(reg_D, reg_X, 0) + SETBYTE(reg_D, reg_Y, 1);
            break;
        case MV_D_XY_:
            reg_X = GETBYTE(reg_D, 0);
            reg_Y = GETBYTE(reg_D, 1);
            break;
        case MV_D_AB_:
            reg_A = GETBYTE(reg_D, 0);
            reg_B = GETBYTE(reg_D, 1);
            break;
        case MV_DL_X_:
            reg_X = GETBYTE(reg_D, 0);
            break;
        case MV_DH_Y_:
            reg_Y = GETBYTE(reg_D, 1);

            break;

            // JUMP TO POINT
        case JMP____I:

            pc = COMBINE(args[0], args[1]);
            break;
        case JMP___AB:

            pc = COMBINE(reg_A, reg_B);
            break;
        case JMP_PC_I:
            pc += args[0];
            break;

        case JMP____D:
            pc = reg_D;
            break;
            // COMPARE TWO DIFFERENT VALUES
        case CMP__A_B:
            reg_flg &= 0xfc;
            if (reg_A < reg_B)
                SET_CONDITON_FLAG(A_IS_SMALLER_B);
            else if (reg_A > reg_B)
                SET_CONDITON_FLAG(A_IS_GREATER_B);
            else if (reg_A != reg_B)
                SET_CONDITON_FLAG(A_NOT_EQUAL__B);
            else
                SET_CONDITON_FLAG(A_EQLUAL__TO_B);
            break;
        case CMP__I_A:
            reg_flg &= 0xfc;
            if (reg_A < args[0])
                SET_CONDITON_FLAG(A_IS_SMALLER_B);
            else if (reg_A > args[0])
                SET_CONDITON_FLAG(A_IS_GREATER_B);
            else if (reg_A != args[0])
                SET_CONDITON_FLAG(A_NOT_EQUAL__B);
            else
                SET_CONDITON_FLAG(A_EQLUAL__TO_B);
            break;

        // JUMP IF CONDITION IS TRUE
        // Equal to
        case JMP_EQ_I:

            if (GET_CONDITON_FLAG == A_EQLUAL__TO_B)
                pc = COMBINE(args[0], args[1]);
            break;
        // Greater Then to
        case JMP_GT_I:
            if (GET_CONDITON_FLAG == A_IS_GREATER_B)
                pc = COMBINE(args[0], args[1]);
            break;
        // Less Then
        case JMP_LT_I:
            if (GET_CONDITON_FLAG == A_IS_SMALLER_B)
                pc = COMBINE(args[0], args[1]);

            break;
        case JMP_NE_I:
            if (GET_CONDITON_FLAG == A_NOT_EQUAL__B)
                pc = COMBINE(args[0], args[1]);

            break;
        // COMPARE WITH IMMEDATE VALUES
        case CMP__I_I:
            reg_flg &= 0xfc;
            if (args[0] < args[1])
                SET_CONDITON_FLAG(A_IS_SMALLER_B);
            else if (args[0] > args[1])
                SET_CONDITON_FLAG(A_IS_GREATER_B);
            else if (args[0] != args[1])
                SET_CONDITON_FLAG(A_NOT_EQUAL__B);
            else
                SET_CONDITON_FLAG(A_EQLUAL__TO_B);
            break;

        case PUSH_A__:
            memory[sp++] = reg_A;
            break;
        case PUSH_I__:
            memory[sp++] = args[0];
            break;
        case POP_A___:
            reg_A = memory[--sp];
            break;
        case PEAK_A__:
            reg_A = memory[sp];
            break;
        case PUSH_RP_:
            memory[sp++] = reg_A;
            memory[sp++] = reg_A;
            break;
        case POP_RP__:
            rp = COMBINE(memory[sp--], memory[sp--]);
            break;
        case __STOP__:
            goto out;
            break;

        case CALLF_AB:
            rp = pc;
            print_range(memory, 0, 80, 1024);
            pc = COMBINE(reg_A, reg_B);
            break;
        case CALLF_II:
            rp = pc;
            pc = COMBINE(args[0], args[1]);
            break;
        case CALLF_D_:
            rp = pc;
            pc = reg_D;
            break;
        case RETURN__:

            pc = rp;
            break;
        case INC_A___:
            reg_A++;
            break;
        case DEC_X___:
            reg_X--;
            break;
        case DEC_Y___:
            reg_Y--;
            break;
        case OUTPUT_A:
            printf("-------OUTPUT_A------------%d\n", reg_A);
            break;
        case PRINTC_A:
            printf("-------PRINT_A------------%c\n", reg_A);
            break;
        case OUTPUT_D:
            printf("-------OUTPUT_D------------%d\n", reg_D);
            break;
        default:

            break;
        }
        pc++;
        i++;
    }
out:

    for (int a = 0; a < CYCLES; a++)
    {
        //DPRINTF("%04d %s\n", a, str_instructions[inst[a]]);
    }
    exit(1);
}
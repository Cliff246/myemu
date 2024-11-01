#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "inc/me_myemu.h"

void printrange(char* range, size_t start, size_t stop,size_t size)
{
    if(size<stop)
    {
        return;
    }
    int instruction = range[start];
    int arglen = 0;
    for(size_t i = start; i < stop; i++)
    {
        print_bin(range[i], 8, 0);
        printf(" %02lld  %04d  ", i, range[i]);
        
        print_hex(range[i], 0);
     
        if(range[i] >= 0 && range[i] < sizeof(str_instructions)/sizeof(*str_instructions))
        {
            if(arglen > 0)
            {
                arglen--;
                printf(" %d", range[i]);
            }
            else{

            
                printf(" %s", str_instructions[(size_t)range[(int)i]]);
                instruction = range[i];
                arglen = len_instructions[instruction];
            }
        }
        printf("\n");
    }
    
}










void emulate(char *program, size_t size)
{
    #define EMULATOR_RAM 1024
    char memory[EMULATOR_RAM];
    ushort pc = 0, sp = 1024 - 100;
    char reg_A = 0, reg_B = 0, reg_C = 0, reg_flg = 0;
    memset(memory, 0, EMULATOR_RAM * sizeof(*memory));
    
    memcpy(memory, program, sizeof(program)*size);
    #define GET_CONDITON_FLAG ((char)reg_flg & 3 )
    #define SET_CONDITON_FLAG(x) (reg_flg += ((char)x & 3))
    #define A_NOT_EQUAL__B 0 
    #define A_EQLUAL__TO_B 1
    #define A_IS_GREATER_B 2
    #define A_IS_SMALLER_B 3
    #define NEXT (memory[++pc])
    #define UPDATENEXT DPRINTF("item %d at address%d\n", memory[pc],  pc)
    
    for(size_t i = 0; i < 10; i++)
    {
        
        char args[3];
        ushort startpc = pc;
        int opcode = startpc[memory], 
            oplen = len_instructions[opcode];
        
        if(oplen >= 3)
        {
            break;
        }
        DPRINTF("pc ->%d\n", pc);
        DPRINTF("A= %d, B= %d, C= %d\n", reg_A, reg_B, reg_C);
        
        DPRINTF("opcode ->%s oplen ->%d\n", str_instructions[opcode], oplen);
        
        for(int i = 0; i < oplen; i++)
        {
            args[i] = NEXT;
            UPDATENEXT;            
        }
        print_bin(reg_flg, 8, 1);
        
        switch(opcode)
        {
            
            //MEMSET
            case M_SET_A_:

                memory[COMBINE(args[0], args[1])] = reg_A;
                break;
            //MEMGET
            case M_GET_AB:

                reg_A = memory[COMBINE(reg_A, reg_B)];
                break;
            //ADD WITH CARRY
            case ADDC_ABC:
                reg_C = reg_B + reg_A;
                
                break;
            case ADDC_AIC:
                reg_C = args[0] + reg_A;
                break;
            case ADDC_IIC:
           
                reg_C = args[0] + args[1];
                break;
            //SUB WITH CARRY
            case SUBC_AIC:
                reg_C = reg_A -args[0] ;
                break;
            case SUBC_IIC:
                reg_C = args[0] - args[1];
                break;
            case SUBC_ABC:
                reg_C = reg_A - reg_B;
                
                break;
            //SET REGISTER USING IMEDIATE
            case SET_I__A:
                reg_A = args[0];
                break;
            case SET_I__B:
                reg_B = args[0];
                break;

            //SWAP REGISTERS
            case MOVE_A_B:
                reg_B = reg_A;
                break;
            case MOVE_B_A:
                reg_A = reg_B;
                break;
            case MOVE_B_C:
                reg_C = reg_B;
                break;
            case MOVE_A_C:
                reg_C = reg_A;
                break;
            case MOVE_C_A:
                reg_A = reg_C;
                break;
            case MOVE_C_B:
                reg_B = reg_A;
                break;
            //JUMP TO POINT
            case JMP____I:

                pc = COMBINE(args[0], args[1]);
                break;
            case JMP__A_B:
                
                pc = COMBINE(reg_A, reg_B);
                break;
            //COMPARE TWO DIFFERENT VALUES
            case CMP__A_B:
                reg_flg &= 0xfc;
                if (reg_A < reg_B)
                    SET_CONDITON_FLAG(A_IS_SMALLER_B);
                else if(reg_A > reg_B)
                    SET_CONDITON_FLAG(A_IS_GREATER_B); 
                else if(reg_A != reg_B)
                    SET_CONDITON_FLAG(A_NOT_EQUAL__B);
                else
                    SET_CONDITON_FLAG(A_EQLUAL__TO_B);
                
                break;
            case CMP__I_A:
                reg_flg &= 0xfc;
                if (reg_A < args[0])
                    SET_CONDITON_FLAG(A_IS_SMALLER_B);
                else if(reg_A > args[0])
                    SET_CONDITON_FLAG(A_IS_GREATER_B); 
                else if(reg_A != args[0])
                    SET_CONDITON_FLAG(A_NOT_EQUAL__B);
                else
                    SET_CONDITON_FLAG(A_EQLUAL__TO_B);
                break;

            //JUMP IF CONDITION IS TRUE
            //Equal to 
            case JMP_EQ_A:
                
                if(GET_CONDITON_FLAG == A_EQLUAL__TO_B)
                    pc = COMBINE(args[0], args[1]);   
                break;
            //Greater Then to
            case JMP_GT_A:
                if(GET_CONDITON_FLAG == A_IS_GREATER_B)
                    pc = COMBINE(args[0], args[1]);
                break;
            //Less Then
            case JMP_LT_A:
                if(GET_CONDITON_FLAG == A_IS_SMALLER_B)
                    pc = COMBINE(args[0], args[1]);

                break;
            case JMP_NE_A:
                if(GET_CONDITON_FLAG == A_NOT_EQUAL__B)
                    pc = COMBINE(args[0], args[1]);

                break;
            //COMPARE WITH IMMEDATE VALUES
            case CMP__I_I:
                reg_flg &= 0xfc;
                if (args[0] < args[1])
                    SET_CONDITON_FLAG(A_IS_SMALLER_B);
                else if(args[0] > args[1])
                    SET_CONDITON_FLAG(A_IS_GREATER_B); 
                else if(args[0] != args[1])
                    SET_CONDITON_FLAG(A_NOT_EQUAL__B);
                else
                    SET_CONDITON_FLAG(A_EQLUAL__TO_B);
                break;

            case PUSH_A__:
                memory[++sp] = reg_A;
                break;
            case PUSH_I__:
                memory[++sp] = args[0];
                break;
            case POP_A___:
                reg_A = memory[--sp];
                break;

            case __STOP__:
                
                return;


            case CALLF_AB:

                memory[++sp] = SPLITL(pc);
                memory[++sp] = SPLITR(pc);
                printrange(memory, 0, 80, 1024);
                pc = COMBINE(reg_A, reg_B);
                break;
            case CALLF_II:
                memory[++sp] = SPLITL(pc);
                memory[++sp] = SPLITR(pc);
                pc = COMBINE(args[0], args[1]);

                break;
            case RETURN__:
                
                pc = COMBINE((memory[--sp]), (memory[--sp]));
                break;
            case INC_A___:
                reg_A++;
            

                break;
            default :
                
                break;

        }
        pc++;

    }
}

const char program1[] = {
    SET_I__A,
    (char)50,
    M_SET_A_,
    SPLITL(50),
    SPLITR(50),
    M_GET_AB,
    SPLITL(50),
    SPLITR(50),
    MOVE_A_B,
    ADDC_ABC,
    MOVE_B_A,
    CMP__A_B,
    JMP_EQ_A,
    SPLITL(0),
    SPLITR(0),
    __STOP__,
};

const char program2[] = {
    SET_I__A,
    (char)1,
    SET_I__B,
    (char)100,
    PUSH_A__,
    MOVE_B_A,
    PUSH_A__,
    POP_A___,
    MOVE_A_B,
    POP_A___,
    ADDC_ABC,
    __STOP__,
};

const char program3[] = {
    SET_I__A,
    (char)20,
    SET_I__B,
    (char)30,
    PUSH_A__,
    MOVE_A_B,
    POP_A___,
    __STOP__,
};

const char program4[] =
    {
        SET_I__A,
        SPLITL(30),
        SET_I__B,
        SPLITR(30),
        CALLF_AB,
        PUSH_A__,
        (char)100,
        MOVE_A_B,
        PUSH_A__,
        (char)200,
        ADDC_ABC,
        POP_A___,
        POP_A___,
        RETURN__,
};

const char program5[] =
    {
        SET_I__A,
        1,
        SET_I__B,
        1,
        INC_A___,
        ADDC_ABC,

        __STOP__,
};

int getdata(void *ptr)
{
    int *iptr = (int *)ptr;
    return *iptr;
}

void *makedata(int x)
{
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = x;
    return (void *)ptr;
}

void freedata(void *data)
{
    free(data);
}

int main(int argc, char *argv[])
{
    //p_hashtable_t table = new_hash_table(1000, freedata);
 //
    //addto_hash_table(table, "hello world", makedata(10));
    //addto_hash_table(table, "test", makedata(123));
    //int i1 = getdata(getdata_from_hash_table(table, "hello world"));
    //int i2 = getdata(getdata_from_hash_table(table, "test"));
    //printf("%d %d\n", i1, i2);
    //
//
    //print_hash_table(table);
//
//
    //p_hashelem_t elem2 = remove_from_hash_table(table, "test");
    //p_hashelem_t elem1 = remove_from_hash_table(table, "hello world");
    //print_hash_table(table);
    //free_hash_element(elem1, freedata);
    //free_hash_element(elem2, freedata);

    // p_tok_t token = split_str_into_tokens("hello,world", ',', 0);
    //print_p_toks_st(token);
    //free_p_toks_st(&token);
    FILE *fptr = fopen("EMULATOR", "");
    assemble("EMU.txt", 0);
    //emulate(program1, sizeof(program1));
    //printf("_______________________\n");
    //emulate(program2, sizeof(program2));
    //printf("_______________________\n");
    //emulate(program3, sizeof(program3));
    //printf("_______________________\n");
    //emulate(program4, sizeof(program4)/sizeof(*program4));
    //emulate(program5, sizeof(program5));
    //printf("______________________\n");
}

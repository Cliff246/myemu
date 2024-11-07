#ifndef __ME_CONSTANTS__
#define __ME_CONSTANTS__

#include "me_myemu.h"
#include "me_commons.h"
#include "me_cpu.h"

static const char program1[] = {
    SET_I__A,
    (char)50,
    M_SET_A_,
    SPLITL(50),
    SPLITR(50),
    M_GET_AB,
    SPLITL(50),
    SPLITR(50),
    MV_A_B__,
    ADDC_ABC,
    MV_B_A__,
    CMP__A_B,
    JMP_EQ_I,
    SPLITL(0),
    SPLITR(0),
    __STOP__,
};
// 9, 50, 2, 50, 0, 1, 50, 0, 12, 3, 11, 19, 23, 0, 0, 31

static const char program2[] = {
    SET_I__A,
    (char)1,
    SET_I__B,
    (char)100,
    PUSH_A__,
    MV_B_A__,
    PUSH_A__,
    POP_A___,
    MV_A_B__,
    POP_A___,
    ADDC_ABC,
    __STOP__,
};

static const char program3[] = {
    SET_I__A,
    (char)20,
    SET_I__B,
    (char)30,
    PUSH_A__,
    MV_A_B__,
    POP_A___,
    __STOP__,
};

static const char program4[] =
    {
        SET_I__A,
        SPLITL(30),
        SET_I__B,
        SPLITR(30),
        CALLF_AB,
        PUSH_A__,
        (char)100,
        MV_A_B__,
        PUSH_A__,
        (char)200,
        ADDC_ABC,
        POP_A___,
        POP_A___,
        RETURN__,
};

static const char program5[] =
    {
        SET_I__A,
        1,
        SET_I__B,
        1,
        INC_A___,
        ADDC_ABC,
        __STOP__,
};

static const char program6[] = {
    9, 0, 10, 0, 9, 15, 10, 1, 3, 17, 4, 0};

#endif
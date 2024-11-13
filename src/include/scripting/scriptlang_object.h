#ifndef __SCRIPTLANG_OBJECT__
#define __SCRIPTLANG_OBJECT__

typedef enum scriptlang_type
{
    type_none,
    type_int,
    type_float,
    type_string,
    type_tuple,
    type_boolean,
    type_class,
}sl_type_t;

typedef struct scriptlang_object
{
    void *m_valueptr;
    sl_type_t m_type;
}sl_object_t;

#endif
#include "hooligan.h"

Type *new_type_int()
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = INT;
    return ty;
}

Type *new_type_ptr(Type *ptr_to)
{
    return NULL; // TODO
}

Type *new_type_array(Type *ptr_to)
{
    return NULL; // TODO
}

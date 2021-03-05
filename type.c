#include "hooligan.h"

Type *new_type_int()
{
    static Type *ty;
    if (!ty)
    {
        ty = calloc(1, sizeof(Type));
        ty->ty = INT;
    }
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

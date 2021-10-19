#include "../hooligan.h"

bool is_int(Type *ty)
{
    return ty->ty == INT;
}

bool is_float(Type *ty)
{
    return ty->ty == FLOAT;
}

bool is_char(Type *ty)
{
    return ty->ty == CHAR;
}

bool is_int_or_char(Type *ty)
{

    return is_int(ty) || is_char(ty);
}

bool is_ptr(Type *ty)
{
    if (!ty->ptr_to)
    {
        return false;
    }
    return true;
}

int calc_bytes(Type *ty)
{
    switch (ty->ty)
    {
    case INT:
        return 4;
    case FLOAT:
        return 4;
    case PTR:
        return 8;
    case ARRAY:
        return calc_bytes(ty->ptr_to) * ty->array_size;
    case CHAR:
        return 1;
    case STRUCT:
        return ty->size;
    case ENUM:
        return 4;
    }
}

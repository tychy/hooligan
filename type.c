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

Type *new_type_char()
{
    static Type *ty;
    if (!ty)
    {
        ty = calloc(1, sizeof(Type));
        ty->ty = CHAR;
    }
    return ty;
}

Type *new_type_string()
{
    //new_type_ptr(new_type_char())で代用できるがキャッシュできる分はやい？
    //わかりやすさは勝っていると思う
    static Type *ty;
    if (!ty)
    {
        Type *c = new_type_char();
        ty = calloc(1, sizeof(Type));
        ty->ty = PTR;
        ty->ptr_to = c;
    }
    return ty;
}

Type *new_type_ptr(Type *ptr_to)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = PTR;
    ty->ptr_to = ptr_to;
    return ty;
}

Type *new_type_array(Type *ptr_to, size_t size)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = ARRAY;
    ty->ptr_to = ptr_to;
    ty->array_size = size;

    return ty;
}

bool is_int(Type *ty)
{
    return ty->ty == INT;
}

bool is_char(Type *ty)
{
    return ty->ty == CHAR;
}

bool is_int_or_char(Type *ty)
{

    return is_int(ty) || is_char(ty);
}

int calc_bytes(Type *ty)
{
    switch (ty->ty)
    {
    case INT:
        return 4;
    case PTR:
        return 8;
    case ARRAY:
        return calc_bytes(ty->ptr_to) * ty->array_size;
    case CHAR:
        return 1;
    }
}

Type *determine_expr_type(Type *lhs, Type *rhs)
{
    if (not(is_int_or_char(lhs)) && not(is_int_or_char(rhs)))
        return NULL;
    else if (is_int_or_char(lhs) && not(is_int_or_char(rhs)))
        return rhs;
    else if (not(is_int_or_char(lhs)) && is_int_or_char(rhs))
        return lhs;
    else if (is_int_or_char(lhs) && is_int_or_char(rhs))
        return new_type_int();
}

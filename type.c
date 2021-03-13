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

Type *new_type_struct()
{
    static Type *ty;
    if (!ty)
    {
        ty = calloc(1, sizeof(Type));
        ty->ty = STRUCT;
    }
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
    case STRUCT:
        return ty->size;
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

Type *find_type(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Type *type = scope->types; type; type = type->next)
        {
            if (type->length == tok->length && memcmp(type->name, tok->string, type->length) == 0)
            {
                return type;
            }
        }
    }
    return NULL;
}

Type *def_type(Token *tok, Type *ty, bool is_local)
{
    Type *new_type = calloc(1, sizeof(Type));
    new_type->name = tok->string;
    new_type->length = tok->length;
    new_type->ty = ty->ty;
    new_type->ptr_to = ty->ptr_to;
    new_type->array_size = ty->array_size;
    new_type->members = ty->members;
    new_type->size = ty->size;
    new_type->next = ctx->scope->types;
    ctx->scope->types = new_type;
    return new_type;
}

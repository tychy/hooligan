#include "../hooligan.h"

Type *new_type_int()
{
    Type *ty;
    ty = calloc(1, sizeof(Type));
    ty->ty = INT;
    ty->is_const = false;
    return ty;
}

Type *new_type_float()
{
    Type *ty;
    ty = calloc(1, sizeof(Type));
    ty->ty = FLOAT;
    ty->is_const = false;
    return ty;
}

Type *new_type_char()
{
    Type *ty;
    ty = calloc(1, sizeof(Type));
    ty->ty = CHAR;
    ty->is_const = false;
    return ty;
}

Type *new_type_string()
{
    //new_type_ptr(new_type_char())で代用できるがキャッシュできる分はやい？
    //わかりやすさは勝っていると思う
    Type *ty;
    Type *c = new_type_char();
    ty = calloc(1, sizeof(Type));
    ty->ty = PTR;
    ty->ptr_to = c;
    ty->is_const = false;
    return ty;
}

Type *new_type_ptr(Type *ptr_to)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = PTR;
    ty->ptr_to = ptr_to;
    ty->is_const = false;
    return ty;
}

Type *new_type_array(Type *ptr_to, size_t size)
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = ARRAY;
    ty->ptr_to = ptr_to;
    ty->array_size = size;
    ty->is_const = false;
    return ty;
}

Type *new_type_struct()
{
    Type *ty;
    ty = calloc(1, sizeof(Type));
    ty->ty = STRUCT;
    ty->size = -1;
    ty->is_const = false;
    return ty;
}

Type *new_type_enum()
{
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = ENUM;
    ty->is_const = false;
    return ty;
}

Type *new_type_void()
{
    Type *ty;
    ty = calloc(1, sizeof(Type));
    ty->ty = VOID;
    return ty;
}

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

Type *determine_expr_type(Type *lhs, Type *rhs)
{
    if (!(is_int_or_char(lhs)) && !(is_int_or_char(rhs)))
    {
        if (is_ptr(lhs) && is_ptr(rhs))
        {
            return lhs;
        }
        if (is_float(lhs) && is_float(rhs))
        {
            return lhs;
        }
        return NULL;
    }
    else if (is_int_or_char(lhs) && !(is_int_or_char(rhs)))
        return rhs;
    else if (!(is_int_or_char(lhs)) && is_int_or_char(rhs))
        return lhs;
    else if (is_int_or_char(lhs) && is_int_or_char(rhs))
        return new_type_int();
}

Type *find_defined_type(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Type *type = scope->defined_type; type; type = type->next_defined)
        {
            if (type->length == tok->length && memcmp(type->name, tok->string, type->length) == 0)
            {
                return type;
            }
        }
    }
    return NULL;
}

Type *add_defined_type(Token *tok, Type *ty, bool is_local)
{
    ty->name = tok->string;
    ty->length = tok->length;
    ty->next_defined = ctx->scope->defined_type;
    ctx->scope->defined_type = ty;
    return ty;
}

Type *find_tagged_type(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Type *type = scope->tagged_types; type; type = type->next_tagged)
        {
            if (type->tag_length == tok->length && memcmp(type->tag_name, tok->string, type->tag_length) == 0)
            {
                return type;
            }
        }
    }
    return NULL;
}

Type *add_tagged_type(Token *tok, Type *ty, bool is_local)
{
    ty->tag_name = tok->string;
    ty->tag_length = tok->length;
    ty->next_tagged = ctx->scope->tagged_types;
    ctx->scope->tagged_types = ty;
    return ty;
}

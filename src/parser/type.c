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
    // new_type_ptr(new_type_char())で代用できるがキャッシュできる分はやい？
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
            if (type->length == tok->len && memcmp(type->name, tok->str, type->length) == 0)
            {
                return type;
            }
        }
    }
    return NULL;
}

Type *add_defined_type(Token *tok, Type *ty, bool is_local)
{
    ty->name = tok->str;
    ty->length = tok->len;
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
            if (type->tag_length == tok->len && memcmp(type->tag_name, tok->str, type->tag_length) == 0)
            {
                return type;
            }
        }
    }
    return NULL;
}

Type *add_tagged_type(Token *tok, Type *ty, bool is_local)
{
    ty->tag_name = tok->str;
    ty->tag_length = tok->len;
    ty->next_tagged = ctx->scope->tagged_types;
    ctx->scope->tagged_types = ty;
    return ty;
}

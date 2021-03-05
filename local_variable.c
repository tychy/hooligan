#include "hooligan.h"

Var *locals;

Var *find_lvar(Token *tok)
{
    for (Var *lvar = locals; lvar; lvar = lvar->next)
    {
        if (lvar->length == tok->length && memcmp(lvar->name, tok->string, lvar->length) == 0)
        {
            return lvar;
        }
    }
    return NULL;
}

Var *globals;
Var *find_gvar(Token *tok)
{
    for (Var *gvar = globals; gvar; gvar = gvar->next)
    {
        if (gvar->length == tok->length && memcmp(gvar->name, tok->string, gvar->length) == 0)
        {
            return gvar;
        }
    }
    return NULL;
}

int def_lvar(Token *tok, Type *ty)
{
    int offset;
    Var *new_lvar = calloc(1, sizeof(Var));

    new_lvar->length = tok->length;
    new_lvar->name = tok->string;
    new_lvar->ty = ty;
    int var_size = calc_bytes(ty);

    if (locals)
    {
        offset = locals->offset + var_size;
    }
    else
    {
        offset = var_size;
    }

    new_lvar->offset = offset;
    new_lvar->next = locals;
    locals = new_lvar;
    return offset;
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
    }
}

#include "hooligan.h"

Var *locals;
Var *globals;

Var *find_var(Token *tok)
{
    for (Var *lvar = locals; lvar; lvar = lvar->next)
    {
        if (lvar->length == tok->length && memcmp(lvar->name, tok->string, lvar->length) == 0)
        {
            return lvar;
        }
    }
    for (Var *gvar = globals; gvar; gvar = gvar->next)
    {
        if (gvar->length == tok->length && memcmp(gvar->name, tok->string, gvar->length) == 0)
        {
            return gvar;
        }
    }
    return NULL;
}

Var *def_var(Token *tok, Type *ty, bool is_local)
{
    int offset;
    Var *new_type = calloc(1, sizeof(Var));

    new_type->length = tok->length;
    new_type->name = tok->string;
    new_type->ty = ty;
    if (is_local)
    {
        int var_size = calc_bytes(ty);
        if (locals)
        {
            offset = locals->offset + var_size;
        }
        else
        {
            offset = var_size;
        }
        new_type->offset = offset;
        new_type->next = locals;
        locals = new_type;
    }
    else
    {
        new_type->next = globals;
        globals = new_type;
    }
    new_type->is_local = is_local;
    return new_type;
}

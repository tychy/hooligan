#include "hooligan.h"

Var *locals;
static Var *find_lvar(Token *tok)
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
static Var *find_gvar(Token *tok)
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

Var *find_var(Token *tok, bool is_local)
{
    if (is_local)
        return find_lvar(tok);
    else
        return find_gvar(tok);
}

static int def_lvar(Token *tok, Type *ty)
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

static int def_gvar(Token *tok, Type *ty)
{
    Var *new_gvar = calloc(1, sizeof(Var));
    new_gvar->name = tok->string;
    new_gvar->length = tok->length;
    new_gvar->ty = ty;
    new_gvar->next = globals;
    globals = new_gvar;
    return 0;
}

int def_var(Token *tok, Type *ty, bool is_local)
{
    if (is_local)
        return def_lvar(tok, ty);
    else
        return def_gvar(tok, ty);
}



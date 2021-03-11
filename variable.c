#include "hooligan.h"

int offset;

Var *find_var(Token *tok)
{
    for (Scope *scope = current_scope; scope; scope = scope->prev)
    {
        for (Var *var = scope->variables; var; var = var->next)
        {
            if (var->length == tok->length && memcmp(var->name, tok->string, var->length) == 0)
            {
                return var;
            }
        }
    }
    return NULL;
}

Var *def_var(Token *tok, Type *ty, bool is_local)
{
    Var *new_var = calloc(1, sizeof(Var));

    new_var->length = tok->length;
    new_var->name = tok->string;
    new_var->ty = ty;
    if (is_local)
    {
        int var_size = calc_bytes(ty);
        offset += var_size;
        new_var->offset = offset;
    }
    new_var->next = current_scope->variables;
    current_scope->variables = new_var;
    new_var->is_local = is_local;
    new_var->is_static = false;

    return new_var;
}

Var *def_var_static(Token *tok, Type *ty, bool is_local)
{
    Var *new_var = def_var(tok, ty, is_local);
    new_var->is_static = true;
    return new_var;
}
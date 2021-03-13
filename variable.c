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

Var *def_static_var(Token *tok, Type *ty, bool is_local, int init_val)
{
    Var *new_var = def_var(tok, ty, is_local);
    new_var->is_static = true;
    new_var->label = current_scope->label;
    // nextメンバが上書きされてしまうので二回Varを生成している
    Var *new_static = calloc(1, sizeof(Var));
    new_static->length = tok->length;
    new_static->name = tok->string;
    new_static->ty = ty;
    new_static->next = statics;
    new_static->label = current_scope->label;
    new_static->init_val = init_val;
    new_static->is_static = true;
    new_static->is_local = true;
    statics = new_static;
    return new_var;
}

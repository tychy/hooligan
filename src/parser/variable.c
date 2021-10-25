#include "../hooligan.h"

static Var *find_var(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Var *var = scope->variables; var; var = var->next)
        {
            if (var->length == tok->len && memcmp(var->name, tok->str, var->length) == 0)
            {
                return var;
            }
        }
    }
    return NULL;
}

static Var *def_var(Token *tok, Type *ty, bool is_local, bool is_static)
{
    Var *new_var = calloc(1, sizeof(Var));

    new_var->length = tok->len;
    new_var->name = tok->str;
    new_var->ty = ty;
    if (is_local)
    {
        int var_size = calc_bytes(ty);
        ctx->offset += var_size;
        new_var->offset = ctx->offset;
    }
    new_var->next = ctx->scope->variables;
    ctx->scope->variables = new_var;
    new_var->is_local = is_local;
    new_var->is_static = is_static;
    new_var->label = ctx->scope->label;
    return new_var;
}

static StaticVar *add_static_local_var(Token *tok, Type *ty, int init_val)
{
    StaticVar *new_static = calloc(1, sizeof(StaticVar));
    new_static->length = tok->len;
    new_static->name = tok->str;
    new_static->ty = ty;
    new_static->next = ctx->statics;
    new_static->label = ctx->scope->label;
    new_static->init_val = init_val;
    ctx->statics = new_static;
    return new_static;
}

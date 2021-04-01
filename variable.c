#include "hooligan.h"

Var *find_var(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
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

Var *def_var(Token *tok, Type *ty, bool is_local, bool is_static)
{
    Var *new_var = calloc(1, sizeof(Var));

    new_var->length = tok->length;
    new_var->name = tok->string;
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
    return new_var;
}

Var *def_static_var(Token *tok, Type *ty, bool is_local, int init_val)
{
    // is_local == true && is_static==trueのときのみ呼ばれる
    Var *new_var = def_var(tok, ty, is_local, true);
    new_var->is_static = true;
    new_var->label = ctx->scope->label;
    // nextメンバが上書きされてしまうので二回Varを生成している
    Var *new_static = calloc(1, sizeof(Var));
    new_static->length = tok->length;
    new_static->name = tok->string;
    new_static->ty = ty;
    new_static->next = ctx->statics;
    new_static->label = ctx->scope->label;
    new_static->init_val = init_val;
    new_static->is_static = true;
    new_static->is_local = true;
    ctx->statics = new_static;
    return new_var;
}

Var *find_func(Token *tok)
{
    for (Var *func = ctx->functions; func; func = func->next)
    {
        if (func->length == tok->length && memcmp(func->name, tok->string, func->length) == 0)
        {
            return func;
        }
    }
    return NULL;
}

Var *def_func(Token *tok, Type *ty, int num_args, Type *arg_ty_ls[6], bool is_static)
{
    Var *new_func = calloc(1, sizeof(Var));
    new_func->length = tok->length;
    new_func->name = tok->string;
    new_func->ty = ty;
    new_func->next = ctx->functions;
    new_func->label = ctx->scope->label;
    new_func->is_static = is_static;
    new_func->num_args = num_args;
    for (int i = 0; i < num_args; i++)
    {
        new_func->arg_ty_ls[i] = arg_ty_ls[i];
    }
    ctx->functions = new_func;
    return new_func;
}

Var *find_const(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Var *con = scope->constants; con; con = con->next)
        {
            if (con->length == tok->length && memcmp(con->name, tok->string, con->length) == 0)
            {
                return con;
            }
        }
    }
    return NULL;
}

Var *def_const(Token *tok, int val)
{
    Var *new_const = calloc(1, sizeof(Var));
    new_const->length = tok->length;
    new_const->name = tok->string;
    new_const->ty = new_type_int(); // constはintのみ
    new_const->value = val;
    new_const->next = ctx->scope->constants;
    ctx->scope->constants = new_const;
    return new_const;
}

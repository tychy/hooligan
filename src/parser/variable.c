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

static Var *find_func(Token *tok)
{
    for (Var *func = ctx->functions; func; func = func->next)
    {
        if (func->length == tok->len && memcmp(func->name, tok->str, func->length) == 0)
        {
            return func;
        }
    }
    return NULL;
}

static Var *def_func(Token *tok, Type *ty, int num_args, Type *arg_ty_ls[6], bool is_static, bool has_variable_length_arguments)
{
    Var *new_func = calloc(1, sizeof(Var));
    new_func->length = tok->len;
    new_func->name = tok->str;
    new_func->ty = ty;
    new_func->next = ctx->functions;
    new_func->label = ctx->scope->label;
    new_func->is_static = is_static;
    new_func->num_args = num_args;
    new_func->has_variable_length_arguments = has_variable_length_arguments;
    for (int i = 0; i < num_args; i++)
    {
        new_func->arg_ty_ls[i] = arg_ty_ls[i];
    }
    ctx->functions = new_func;
    return new_func;
}

static Var *find_const(Token *tok)
{
    for (Scope *scope = ctx->scope; scope; scope = scope->prev)
    {
        for (Var *con = scope->constants; con; con = con->next)
        {
            if (con->length == tok->len && memcmp(con->name, tok->str, con->length) == 0)
            {
                return con;
            }
        }
    }
    return NULL;
}

static Var *def_const(Token *tok, int val)
{
    Var *new_const = calloc(1, sizeof(Var));
    new_const->length = tok->len;
    new_const->name = tok->str;
    new_const->ty = new_type_int(); // constはintのみ
    new_const->value = val;
    new_const->next = ctx->scope->constants;
    ctx->scope->constants = new_const;
    return new_const;
}

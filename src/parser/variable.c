#include "../hooligan.h"

static Var *new_var(Token *tok, Type *ty)
{
    static int id = 0;
    Var *var = calloc(1, sizeof(Var));
    var->id = id++;
    var->length = tok->len;
    var->name = tok->str;
    var->ty = ty;
    return var;
}

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
    Var *var = new_var(tok, ty);
    if (is_local)
    {
        int var_size = calc_bytes(ty);
        ctx->offset += var_size;
        var->offset = ctx->offset;
    }
    var->next = ctx->scope->variables;
    ctx->scope->variables = var;
    var->is_local = is_local;
    var->is_static = is_static;
    return var;
}

static Var *new_non_prototyped_function(char *name, int length)
{
    Var *func = calloc(1, sizeof(Var));
    func->name = name;
    func->length = length;
    func->ty = new_type_int();
    return func;
}

static Var *def_func(Token *tok, Type *ty, int num_args, Type *arg_ty_ls[6], bool is_static, bool has_variable_length_arguments)
{
    Var *new_func = new_var(tok, ty);
    new_func->next = ctx->scope->variables;
    new_func->is_static = is_static;
    new_func->num_args = num_args;
    new_func->has_variable_length_arguments = has_variable_length_arguments;
    for (int i = 0; i < num_args; i++)
    {
        new_func->arg_ty_ls[i] = arg_ty_ls[i];
    }
    ctx->scope->variables = new_func;
    return new_func;
}

static Var *def_const(Token *tok, int val)
{
    Var *new_const = new_var(tok, new_type_int()); // constはintのみ
    new_const->value = val;
    new_const->is_computable = true;
    new_const->next = ctx->scope->variables;
    ctx->scope->variables = new_const;
    return new_const;
}

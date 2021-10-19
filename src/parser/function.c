#include "../hooligan.h"

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

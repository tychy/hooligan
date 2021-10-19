#include "../hooligan.h"

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

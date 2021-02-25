#include "hooligan.h"

LVar *locals;

LVar *find_lvar(Token *tok)
{
    for (LVar *lvar = locals; lvar; lvar = lvar->next)
    {
        if (lvar->length == tok->length && memcmp(lvar->name, tok->string, lvar->length) == 0)
        {
            return lvar;
        }
    }
    return NULL;
}

int def_lvar(Token *tok, Type *ty)
{
    int offset;
    LVar *new_lvar = calloc(1, sizeof(LVar));
    new_lvar->length = tok->length;
    new_lvar->name = tok->string;
    new_lvar->ty = ty;
    if (locals)
    {
        offset = locals->offset + 8;
    }
    else
    {
        offset = 8;
    }
    new_lvar->offset = offset;
    new_lvar->next = locals;
    locals = new_lvar;
    return offset;
}

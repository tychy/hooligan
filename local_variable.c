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


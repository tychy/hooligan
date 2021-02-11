#include "hooligan.h"

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char *op)
{
    if (token->kind != TK_OPERATOR || strcmp(token->string, op))
        return false;
    token = token->next;
    return true;
}

void expect(char *op)
{
    if (token->kind != TK_OPERATOR || token->string[0] != op[0])
    {
        error("'%c'ではありません", op[0]);
    }
    token = token->next;
}

int expect_number()
{
    if (token->kind != TK_NUMBER)
        error("数字ではありません");
    int value = token->value;
    token = token->next;
    return value;
}

int expect_var()
{
    if (token->kind != TK_IDENT)
    {
        error("変数ではありません");
    }
    int offset;
    LVar *lvar = find_lvar(token);
    if (lvar)
    {
        offset = lvar->offset;
    }
    else
    {
        LVar *new_lvar = calloc(1, sizeof(LVar));
        new_lvar->length = token->length;
        new_lvar->name = token->string;
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
    }
    token = token->next;
    return offset;
}

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

bool consume_rw(TokenKind tk)
{
    if (token->kind != tk)
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

Token *consume_ident()
{
    if (token->kind != TK_IDENT)
    {
        error("変数ではありません");
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

bool at_eof()
{
    return token->kind == TK_EOF;
}

static char *types[2] = {
    "int",
    "char",
};

bool istype(Token *tok, TypeKind ty)
{
    char *ref = types[ty];
    int reflen = strlen(ref);
    return tok->length == reflen && strncmp(tok->string, ref, reflen) == 0;
}

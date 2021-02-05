#include "hooligan.h"

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char op)
{
    if (token->kind != TK_OPERATOR || token->string[0] != op)
        return false;
    token = token->next;
    return true;
}

void expect(char op)
{
    if (token->kind != TK_OPERATOR || token->string[0] != op)
        error("'%c'ではありません", op);
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

bool at_eof()
{
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->string = str;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')')
        {
            cur = new_token(TK_OPERATOR, cur, p++);
            continue;
        }

        if (isdigit(*p))
        {
            cur = new_token(TK_NUMBER, cur, p);
            cur->value = strtol(p, &p, 10);
            continue;
        }

        error("トークナイズできません");
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
char *operator_list[13] = {
    "==",
    "!=",
    ">=",
    "<=",
    ">",
    "<",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "=",
};

int operator_list_count = sizeof(operator_list) / sizeof(operator_list[0]);

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

void at_eol(char *op)
{
    if (token->kind != TK_EOL || token->string[0] != op[0])
    {
        error("'%c'ではありません", op[0]);
    }
    token = token->next;
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
    int offset = 8 * (1 + *token->string - 'a');
    return offset;
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

bool iseol(char p)
{
    if (p == ';')
    {
        return true;
    }
    return false;
}

bool isident(char p)
{
    if (p >= 'a' && p <= 'z')
    {
        return true;
    }
    return false;
}

bool isoperator(char *p)
{
    for (int i = 0; i < operator_list_count; i++)
    {
        char *str = operator_list[i];
        if (strncmp(p, str, strlen(str)) == 0)
        {
            return true;
        }
    }
    return false;
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
        if (iseol(*p))
        {
            cur = new_token(TK_EOL, cur, p);
            p++;
            continue;
        }
        if (isident(*p))
        {
            cur = new_token(TK_IDENT, cur, p);
            p++;
            continue;
        }

        if (isoperator(p))
        {
            for (int i = 0; i < operator_list_count; i++)
            {

                char *op = operator_list[i];
                if (strncmp(p, op, strlen(op)) == 0)
                {
                    cur = new_token(TK_OPERATOR, cur, op);
                    p += strlen(op);
                }
            }
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

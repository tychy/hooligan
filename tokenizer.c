#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
char *token_list[8] = {
    "==",
    "!=",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
};

int token_list_count = sizeof(token_list) / sizeof(token_list[0]);

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
    if (token->kind != TK_OPERATOR || strcmp(token->string, op))
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

bool istoken(char *p)
{
    for (int i = 0; i < token_list_count; i++)
    {
        char *str = token_list[i];
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

        if (istoken(p))
        {
            for (int i = 0; i < token_list_count; i++)
            {
                char *str = token_list[i];
                if (strncmp(p, str, strlen(str)) == 0)
                {
                    cur = new_token(TK_OPERATOR, cur, str);
                    p += strlen(str);
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

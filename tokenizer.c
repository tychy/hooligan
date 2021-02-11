#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
char *operator_list[14] = {
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
    ";",
};

int operator_list_count = sizeof(operator_list) / sizeof(operator_list[0]);

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

        if (isident(*p))
        {
            int i = 0;
            char *p_top = p;
            while (isident(*p))
            {
                i++;
                p++;
            }
            cur = new_token(TK_IDENT, cur, p_top);
            cur->length = i;
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

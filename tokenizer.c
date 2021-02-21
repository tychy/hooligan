#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
char *operator_list[17] = {
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
    "{",
    "}",
    ",",
};

int operator_list_count = sizeof(operator_list) / sizeof(operator_list[0]);

Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->string = str;
    cur->next = tok;
    return tok;
}

bool isfor(char *p)
{
    if (strncmp(p, "for", 3) == 0 && !isident(*(p + 3)))
    {
        return true;
    }
    return false;
}

bool iswhile(char *p)
{
    if (strncmp(p, "while", 5) == 0 && !isident(*(p + 5)))
    {
        return true;
    }
    return false;
}

bool isif(char *p)
{
    if (strncmp(p, "if", 2) == 0 && !isident(*(p + 2)))
    {
        return true;
    }
    return false;
}

bool iselse(char *p)
{
    if (strncmp(p, "else", 4) == 0 && !isident(*(p + 4)))
    {
        return true;
    }
    return false;
}

bool isreturn(char *p)
{
    if (strncmp(p, "return", 6) == 0 && !isident(*(p + 6)))
    {
        return true;
    }
    return false;
}

bool isident(char p)
{
    if ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z') || p == '_')
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

        if (isreturn(p))
        {
            cur = new_token(TK_RETURN, cur, p);
            cur->length = 6;
            p += 6;
            continue;
        }

        if (isif(p))
        {
            cur = new_token(TK_IF, cur, p);
            cur->length = 2;
            p += 2;
            continue;
        }

        if (iselse(p))
        {
            cur = new_token(TK_ELSE, cur, p);
            cur->length = 4;
            p += 4;
            continue;
        }

        if (isfor(p))
        {
            cur = new_token(TK_FOR, cur, p);
            cur->length = 3;
            p += 3;
            continue;
        }

        if (iswhile(p))
        {
            cur = new_token(TK_WHILE, cur, p);
            cur->length = 5;
            p += 5;
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

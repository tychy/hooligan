#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *operator_list[20] = {
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
    "&",
    "[",
    "]",
};

static int operator_list_count = sizeof(operator_list) / sizeof(operator_list[0]);

static char *reserved_word_list[6] = {
    "return",
    "if",
    "else",
    "for",
    "while",
    "sizeof",
};

static Token *new_token(TokenKind kind, Token *cur, char *str)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->string = str;
    cur->next = tok;
    return tok;
}

static bool isident(char p)
{
    if ((p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z') || p == '_')
    {
        return true;
    }
    return false;
}

static bool isreservedword(char *p, TokenKind kind)
{
    char *word = reserved_word_list[kind];
    int len = strlen(word);
    if (strncmp(p, word, len) == 0 && not(isident(*(p + len))))
        return true;
    return false;
}

static bool isoperator(char *p)
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

        if (isreservedword(p, TK_RETURN))
        {
            cur = new_token(TK_RETURN, cur, p);
            cur->length = 6;
            p += 6;
            continue;
        }

        if (isreservedword(p, TK_IF))
        {
            cur = new_token(TK_IF, cur, p);
            cur->length = 2;
            p += 2;
            continue;
        }

        if (isreservedword(p, TK_ELSE))
        {
            cur = new_token(TK_ELSE, cur, p);
            cur->length = 4;
            p += 4;
            continue;
        }

        if (isreservedword(p, TK_FOR))
        {
            cur = new_token(TK_FOR, cur, p);
            cur->length = 3;
            p += 3;
            continue;
        }

        if (isreservedword(p, TK_WHILE))
        {
            cur = new_token(TK_WHILE, cur, p);
            cur->length = 5;
            p += 5;
            continue;
        }

        if (isreservedword(p, TK_SIZEOF))
        {
            cur = new_token(TK_SIZEOF, cur, p);
            cur->length = 6;
            p += 6;
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

#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *operator_list[27] = {
    "+=",
    "-=",
    "==",
    "!=",
    ">=",
    "<=",
    "->",
    "&&",
    "||",
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
    ".",
    "!",
};

static int operator_list_count = sizeof(operator_list) / sizeof(operator_list[0]);

static char *reserved_word_list[14] = {
    "return",
    "if",
    "else",
    "for",
    "while",
    "sizeof",
    "int",
    "char",
    "struct",
    "typedef",
    "break",
    "continue",
    "static",
    "extern",
};

static int reserved_word_list_count = sizeof(reserved_word_list) / sizeof(reserved_word_list[0]);

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

static bool isreservedword(char *p)
{
    for (TokenKind tk = 0; tk < reserved_word_list_count; tk++)
    {
        char *word = reserved_word_list[tk];
        int len = strlen(word);
        if (strncmp(p, word, len) == 0 && not(isident(*(p + len))))
            return true;
    }
    return false;
}

static TokenKind find_reserved_word(char *p)
{
    for (TokenKind tk = 0; tk < reserved_word_list_count; tk++)
    {
        char *word = reserved_word_list[tk];
        int len = strlen(word);
        if (strncmp(p, word, len) == 0 && not(isident(*(p + len))))
            return tk;
    }
    error("予約語ではありません");
    return -1; // Not Found
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

        if (strncmp(p, "//", 2) == 0)
        {
            p += 2;
            while (*p != '\n')
                p++;
            continue;
        }

        // ブロックコメントをスキップ
        if (strncmp(p, "/*", 2) == 0)
        {
            char *q = strstr(p + 2, "*/");
            if (!q)
                error("コメントが閉じられていません");
            p = q + 2;
            continue;
        }

        if (*p == '"')
        {
            int i = 0;
            p++;
            char *p_top = p;
            while (*p != '"')
            {
                p++;
                i++;
            }
            if (*p != '"')
                error("ダブルクォテーションが閉じていません");

            p++;
            cur = new_token(TK_STRING, cur, p_top);
            cur->length = i;

            continue;
        }

        if(*p == 39){ // ' バックスラッシュによるエスケープを使いたくなかった
        
            p++;
            cur = new_token(TK_CHARACTER, cur, p);
            cur->length = 1;
            cur->value = *p;
            p++;
            if (*p != 39)
            {
                error("シングルクォーテーションが閉じていません");
            }
            p++;
            continue;
        }

        if (isreservedword(p))
        {
            int tk = find_reserved_word(p);
            int len = strlen(reserved_word_list[tk]);
            cur = new_token(tk, cur, p);
            cur->length += len;
            p += len;
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
        error("%sトークナイズできません", p);
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

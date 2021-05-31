#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *operator_list[33] = {
    "++",
    "--",
    "+=",
    "-=",
    "*=",
    "%=",
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
    "%",
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
    ":",
};

static int operator_list_count = 33;

static char *reserved_word_list[19] = {
    "return",
    "if",
    "else",
    "for",
    "while",
    "switch",
    "case",
    "default",
    "sizeof",
    "void",
    "int",
    "char",
    "struct",
    "typedef",
    "break",
    "continue",
    "static",
    "extern",
    "enum",
};

static int reserved_word_list_count = 19;

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
        if (strncmp(p, word, len) == 0 && !(isident(*(p + len))))
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
        if (strncmp(p, word, len) == 0 && !(isident(*(p + len))))
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
        if (strncmp(p, "#", 1) == 0)
        {
            p += 1;
            while (*p != '\n')
                p++;
            continue;
        }
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
            for (;;)
            {
                if (*p == '\\')
                {
                    p++;
                    i++;
                    if (*p == '"')
                    {
                        p++;
                        i++;
                    }
                }
                else if (*p == '"')
                {
                    break;
                }
                else
                {
                    p++;
                    i++;
                }
            }
            if (*p != '"')
                error("ダブルクォテーションが閉じていません");

            p++;
            cur = new_token(TK_STRING, cur, p_top);
            cur->length = i;

            continue;
        }

        if (*p == 39) // 39 = '
        {             // ' バックスラッシュによるエスケープを使いたくなかった
            p++;
            if (*p == 92)
            {
                p++;
                if (*p == 92)
                {
                    cur = new_token(TK_CHARACTER, cur, p);
                    cur->value = '\\';
                }
                else if (*p == '0')
                {
                    cur = new_token(TK_CHARACTER, cur, p);
                    cur->value = '\0';
                }
                else if (*p == 'n')
                {
                    cur = new_token(TK_CHARACTER, cur, p);
                    cur->value = '\n';
                }
                else if (*p == 34)
                {
                    cur = new_token(TK_CHARACTER, cur, p);
                    cur->value = '\"';
                }
                else
                {
                    error("未定義のエスケープ文字です");
                }
            }
            else
            {
                cur = new_token(TK_CHARACTER, cur, p);
                cur->value = *p;
            }
            cur->length = 1;
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
            while (isident(*p) || isdigit(*p))
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
        error1("%sトークナイズできません", p);
    }
    new_token(TK_EOF, cur, p);
    return head.next;
}

Token *tokenize2(PPToken *pptok)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;
    while (pptok)
    {
        switch (pptok->kind)
        {
        case PPTK_CHAR:
            cur = new_token(TK_CHARACTER, cur, pptok->str);
            cur->value = *pptok->str;
            break;
        case PPTK_HN:
            error("未処理のプリプロセッシングトークン列です");
            break;
        case PPTK_IDENT:
            if (isreservedword(pptok->str))
            {
                int tk = find_reserved_word(pptok->str);
                int len = strlen(reserved_word_list[tk]);
                cur = new_token(tk, cur, pptok->str);
                cur->length = len;
            }
            else
            {
                cur = new_token(TK_IDENT, cur, pptok->str);
                cur->length = pptok->len;
            }
            break;
        case PPTK_NUMBER:
            cur = new_token(TK_NUMBER, cur, pptok->str);
            cur->value = pptok->val;
            break;
        case PPTK_PUNC:
            if (!isoperator(pptok->str))
            {
                error("不正なトークンです");
            }
            for (int i = 0; i < operator_list_count; i++)
            {
                char *op = operator_list[i];
                if (strncmp(pptok->str, op, strlen(op)) == 0)
                {
                    cur = new_token(TK_OPERATOR, cur, pptok->str);
                    cur->length = strlen(op);
                }
            }
            break;
        case PPTK_STRING:
            cur = new_token(TK_STRING, cur, pptok->str);
            cur->length = pptok->len;
            break;
        }
        pptok = pptok->next;
    }
    new_token(TK_EOF, cur, "");
    return head.next;
}

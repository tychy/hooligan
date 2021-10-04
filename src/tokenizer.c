#include "hooligan.h"

// note: 文字数の多いものを先に登録する
// note: 要素数を更新する
static char *operator_list[34] = {
    "...",
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

static int operator_list_count = 34;

static char *reserved_word_list[20] = {
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
    "const",
};

static int reserved_word_list_count = 20;

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

static bool isreservedword(char *p, int len)
{
    for (TokenKind tk = 0; tk < reserved_word_list_count; tk++)
    {
        char *word = reserved_word_list[tk];
        if (strncmp(p, word, len) == 0 && len == strlen(word))
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
    error_at(p, "予約語ではありません");
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

// PPTokenをTokenへと変換する
Token *tokenize(PPToken *pptok)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;
    while (pptok != NULL)
    {
        switch (pptok->kind)
        {
        case PPTK_CHAR:
            cur = new_token(TK_CHARACTER, cur, pptok->str);
            cur->value = pptok->val;
            break;
        case PPTK_HN:
            error_at(pptok->str, "未処理のプリプロセッシングトークン列です");
            break;
        case PPTK_IDENT:
            if (isreservedword(pptok->str, pptok->len))
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
                error_at(pptok->str, "不正なトークンです");
            }
            for (int i = 0; i < operator_list_count; i++)
            {
                char *op = operator_list[i];
                if (strncmp(pptok->str, op, strlen(op)) == 0)
                {
                    cur = new_token(TK_OPERATOR, cur, pptok->str);
                    cur->length = strlen(op);
                    break;
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

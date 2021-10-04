#include "hooligan.h"

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

static TokenKind find_reserved_word(char *p, int len)
{
    for (TokenKind tk = 0; tk < reserved_word_list_count; tk++)
    {
        char *word = reserved_word_list[tk];
        if (strncmp(p, word, len) == 0 && len == strlen(word))
            return tk;
    }
    error_at(p, "予約語ではありません");
    return -1; // Not Found
}


static Token *new_token(TokenKind kind, int val, char *str, int len)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->value = val;
    tok->string = str;
    tok->length = len;
    return tok;
}

static Token *convertPPTokenToToken(PPToken *pptok)
{
    TokenKind tk;
    switch (pptok->kind)
    {
    case PPTK_CHAR:
        tk = TK_CHARACTER;
        break;
    case PPTK_HN:
        error_at(pptok->str, "未処理のプリプロセッシングトークン列です");
        break;
    case PPTK_IDENT:
        if (isreservedword(pptok->str, pptok->len))
        {
            tk = find_reserved_word(pptok->str, pptok->len);
        }
        else
        {
            tk = TK_IDENT;
        }
        break;
    case PPTK_NUMBER:
        tk = TK_NUMBER;
        break;
    case PPTK_PUNC:
        tk = TK_OPERATOR;
        break;
    case PPTK_STRING:
        tk = TK_STRING;
        break;
    }
    return new_token(tk, pptok->val, pptok->str, pptok->len);
}

Token *tokenize(PPToken *pptok)
{
    Token head;
    head.next = NULL;
    Token *cur = &head;
    while (pptok != NULL)
    {
        cur->next = convertPPTokenToToken(pptok);
        cur = cur->next;
        pptok = pptok->next;
    }
    cur->next = new_token(TK_EOF, 0, "", 0);
    return head.next;
}

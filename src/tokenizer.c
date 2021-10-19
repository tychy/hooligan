#include "hooligan.h"

static char *reserved_word_list[21] = {
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
    "float",
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

static int reserved_word_list_count = 21;

static bool isreservedword(char *p, int len)
{
    for (ReservedWord rw = 0; rw < reserved_word_list_count; rw++)
    {
        char *word = reserved_word_list[rw];
        if (strncmp(p, word, len) == 0 && len == strlen(word))
            return true;
    }
    return false;
}

static ReservedWord find_reserved_word(char *p, int len)
{
    for (ReservedWord rw = 0; rw < reserved_word_list_count; rw++)
    {
        char *word = reserved_word_list[rw];
        if (strncmp(p, word, len) == 0 && len == strlen(word))
            return rw;
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
            tk = TK_RESERVED_WORD;
            pptok->val = find_reserved_word(pptok->str, pptok->len);
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
    Token *tok = new_token(tk, pptok->val, pptok->str, pptok->len);
    tok->is_float = pptok->is_float;
    tok->float_val = pptok->float_val;
    tok->integer = pptok->integer;
    tok->decimal = pptok->decimal;
    tok->numzero = pptok->numzero;
    return tok;
}

Token *tokenize(PPToken *pptok)
{
    Token head;
    Token *cur = &head;
    while (pptok != NULL)
    {
        cur->next = convertPPTokenToToken(pptok);
        cur = cur->next;
        pptok = pptok->next;
    }
    cur->next = new_token(TK_EOF, 0, "", 0);
    cur->next->is_float = false;
    return head.next;
}

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

Token *tokenize(Token *pptok)
{
    Token *head = pptok;
    Token *cur = head;
    Token *before;
    while (cur != NULL)
    {
        if (cur->kind == TK_IDENT)
        {
            if (isreservedword(cur->str, cur->len))
            {
                cur->kind = TK_RESERVED_WORD;
                cur->val = find_reserved_word(cur->str, cur->len);
            }
        }
        before = cur;
        cur = cur->next;
    }
    before->next = calloc(1, sizeof(Token));
    before->next->kind = TK_EOF;
    return head;
}

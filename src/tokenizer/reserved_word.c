#include "../hooligan.h"

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
        if (strncmp(p, word, len) == 0 && len == h_strlen(word))
            return true;
    }
    return false;
}

static ReservedWord find_reserved_word(char *p, int len)
{
    for (ReservedWord rw = 0; rw < reserved_word_list_count; rw++)
    {
        char *word = reserved_word_list[rw];
        if (strncmp(p, word, len) == 0 && len == h_strlen(word))
            return rw;
    }
    error_at(p, "予約語ではありません");
    return -1; // Not Found
}

#include "hooligan.h"

char *replace_all(char *str, char *what, char *with)
{
    if (!*what)
    {
        return str;
    }
    char *what_pos = str;
    size_t what_len = strlen(what);
    size_t with_len = strlen(with);
    while ((what_pos = strstr(what_pos, what)) != NULL)
    {
        char *remain = what_pos + what_len;
        memmove(what_pos + with_len, remain, strlen(remain) + 1);
        memcpy(what_pos, with, with_len);
    }
    return str;
}

char *preprocess(char *target)
{
    // stdio.h
    replace_all(target, "size_t", "int");
    replace_all(target, "FILE", "int");
    replace_all(target, "SEEK_SET", "0");
    replace_all(target, "SEEK_END", "2");
    return target;
}

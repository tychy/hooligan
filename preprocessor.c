#include "hooligan.h"

// #defineを再現できる
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
    
    replace_all(target, "NULL", "0");
    replace_all(target, "bool", "int");
    replace_all(target, "true", "1");
    replace_all(target, "false", "0");
    return target;
}

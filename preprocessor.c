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

static char *process_include(char *base_dir, char *p)
{
    char *p_start = p;
    while (*p)
    {
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (*p == '#')
        {
            if (memcmp(p, "#include", strlen("#include")) == 0)
            {
                char *p_top = p;
                char *path;
                int path_length = 0;
                bool is_dq = false;
                while (*p != '"' && *p != '<')
                {
                    p++;
                }
                if (*p == '"')
                {
                    is_dq = true;
                }
                p++;
                if (is_dq)
                {
                    while (*p != '"')
                    {
                        path_length++;
                        p++;
                    }
                }
                else
                {
                    while (*p != '>')
                    {
                        path_length++;
                        p++;
                    }
                };
                path = calloc(1, path_length);
                memcpy(path, p - path_length, path_length);
                memset(p_top, ' ', p - p_top + 1); // include文を消す

                if (is_dq)
                {
                    char *full_path = join_str(base_dir, path);
                    char *p_rec = process_include(extract_dir(full_path), read_file(full_path));
                    p = insert_str(p_start, p - p_start, p_rec);
                }
                else
                {
                    path = join_str("include/", path);
                    p = insert_str(p_start, p - p_start, read_file(path));
                }
                p_start = p;
                continue;
            }
            else
            {
                while (*p != '\n')
                {
                    p++;
                }
                p++;
                continue;
            }
        }
        while (*p != '\n')
        {
            p++;
        }
        p++;
    }
    return p_start;
}

char *preprocess(char *base_dir, char *target)
{
    // include文の処理
    target = process_include(base_dir, target);

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

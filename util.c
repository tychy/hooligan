#include "hooligan.h"

bool not(bool flag)
{
    return !flag;
}

void error(char *fmt)
{
    fprintf(stderr, fmt, "");
    fprintf(stderr, "\n");
    exit(1);
}

void error1(char *fmt, char *v1)
{
    fprintf(stderr, fmt, v1);
    fprintf(stderr, "\n");
    exit(1);
}

void error2(char *fmt, char *v1, char *v2)
{
    fprintf(stderr, fmt, v1, v2);
    fprintf(stderr, "\n");
    exit(1);
}

char *join_str(char *pre, char *post)
{
    size_t pre_size = strlen(pre);
    size_t post_size = strlen(post);
    char *res = calloc(1, pre_size + post_size + 2);
    memcpy(res, pre, pre_size);
    memcpy(res + pre_size, post, post_size);
    return res;
}

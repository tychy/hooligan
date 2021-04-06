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

char *insert_str(char *src, int pos, char *target)
{
    char *res = calloc(1, strlen(src) + strlen(target) + 1);
    memcpy(res, src, pos);
    memcpy(res + pos, target, strlen(target));
    memcpy(res + pos + strlen(target), src + pos, strlen(src) - pos);
    return res;
}

char *join_str(char *pre, char *post)
{
    char *res = insert_str(pre, strlen(pre), post);
    return res;
}

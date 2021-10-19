#include "../hooligan.h"

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

void error_at(char *loc, char *msg)
{
    fprintf(stderr, "エラーが発生しました");
    // 前と後ろの改行コードを調べてエラーになった行を抜き出せるようにする
    char *start = loc;
    while (*start != '\n')
    {
        start -= 1;
    }
    char *end = loc;
    while (*end != '\n')
    {
        end += 1;
    }
    int countOfLine = end - start;
    int countToLoc = loc - start;
    fprintf(stderr, "%.*s\n", countOfLine, start);
    fprintf(stderr, "%*s", countToLoc, " "); // pos個の空白を出力
    fprintf(stderr, "^ ");
    fprintf(stderr, "%s", msg);
    fprintf(stderr, "%s", end);
    exit(1);
}

char *insert_str(char *src, int pos, char *target)
{
    char *res = calloc(1, h_strlen(src) + h_strlen(target) + 1);
    memcpy(res, src, pos);
    memcpy(res + pos, target, h_strlen(target));
    memcpy(res + pos + h_strlen(target), src + pos, h_strlen(src) - pos);
    return res;
}

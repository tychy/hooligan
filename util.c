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

char *read_file(char *path)
{
    // ファイルを開く
    FILE *fp = fopen(path, "r");
    if (!fp)
        error1("cannot open %s", path);

    // ファイルの長さを調べる
    if (fseek(fp, 0, SEEK_END) == -1)
        error1("%s: fseek", path);
    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1)
        error1("%s: fseek", path);

    // ファイル内容を読み込む
    char *buf = calloc(1, size + 2);
    fread(buf, size, 1, fp);

    // ファイルが必ず"\n\0"で終わっているようにする
    if (size == 0 || buf[(size - 1)] != '\n')
        buf[size++] = '\n';
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

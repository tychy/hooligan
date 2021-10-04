#include "hooligan.h"

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

// 文字列をファイル名と解釈し拡張子を取り除く
char *remove_extension(char *p)
{
    int preLength = strlen(p);
    if (strchr(p, '.') != NULL)
    {
        preLength = strchr(p, '.') - p;
    }
    char *res = calloc(1, preLength + 1);
    memcpy(res, p, preLength);
    return res;
}

// 文字列をパスと解釈しファイル名を取り出す
char *extract_filename(char *p)
{
    int preLength = 0;
    if (strrchr(p, '/') != NULL)
    {
        preLength = strrchr(p, '/') - p + 1;
    }
    int postLength = strlen(p) - preLength;
    char *p1 = calloc(1, preLength + 1);
    memcpy(p1, p, preLength);
    char *p2 = calloc(1, postLength + 1);
    memcpy(p2, p + preLength, postLength);
    return p2;
}

// 文字列をパスと解釈しディレクトリ名を取り出す
char *extract_dir(char *p)
{
    int preLength = 0;
    if (strrchr(p, '/') != NULL)
    {
        preLength = strrchr(p, '/') - p + 1;
    }
    int postLength = strlen(p) - preLength;
    char *p1 = calloc(1, preLength + 1);
    memcpy(p1, p, preLength);
    char *p2 = calloc(1, postLength + 1);
    memcpy(p2, p + preLength, postLength);
    return p1;
}

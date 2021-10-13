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
/*
int f2bin(float x)
{
    if (x == 0.0)
    {
        return 0;
    }
    int bin[32]; // IEEE754の表現
    int mid[24]; // 中間表現では2進数だがIEEE754ではない

    for (int i = 0; i < 32; i++)
    {
        bin[i] = 0;
    }

    for (int i = 0; i < 24; i++)
    {
        mid[i] = 0;
    }

    // 符号部の処理
    if (x < 0.0)
    {
        bin[0] = 1;
        x *= -1;
    }

    // xを2進数に変換
    // 23桁まで計算

    // 整数部の計算
    // 割れる最大の2^nを見つける
    int n;
    if (x <= 1.0)
    {
        n = 0;
    }
    else
    {
        float power = 1.0;
        n = 1;
        while (x >= 2.0 * power)
        {
            power *= 2.0;
            n += 1;
        }

        for (int i = 0; i < n; i++)
        {
            if (x >= power)
            {
                mid[i] = 1;
                x = x - power;
            }
            else
            {
                mid[i] = 0;
            }
            power /= 2.0;
        }
    }
    // ここでは x<= 1.0になっている

    int i = n;
    if (n == 0)
    {
        while (x < 1.0 && x != 0.0)
        {
            x *= 2.0;
            n -= 1;
        }
    }
    else
    {
        x *= 2.0;
        n = n - 1;
    }

    for (; i < 24; i++)
    {
        if (x == 0.0)
        {
            break;
        }

        if (x >= 1.0)
        {
            mid[i] = 1;
            x = x - 1.0;
        }
        x *= 2.0;
    }

    // IEEE754への変換

    // 指数部
    int keta = 127 + n;
    int power = 128;
    for (int j = 1; j < 9; j++)
    {
        if (keta >= power)
        {
            bin[j] = 1;
            keta -= power;
        }
        power = power / 2;
    }

    // 仮数部分
    for (int j = 9; j < 32; j++)
    {
        // bin[9]は必ず0
        bin[j] = mid[j - 8];
    }
    */
    /*
    for (int j = 0; j < 32; j++)
    {
        if (j == 31)
        {
            printf("%d\n", bin[j]);
        }
        else
        {
            printf("%d", bin[j]);
        }
    }
    for (int j = 0; j < 32; j++)
    {
        if (j == 0)
        {
            printf("s");
        }
        else if (j < 9)
        {
            printf("e");
        }
        else
        {
            printf("_");
        }
    }
    printf("\n");
    */
   /*
    // 10進数への変換
    int res = 0;
    power = 1;
    for (int j = 0; j < 32; j++)
    {
        res += bin[31 - j] * power;
        power *= 2;
    }
    //printf("%d\n", res);
    return res;
}
*/
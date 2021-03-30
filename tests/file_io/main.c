#include <stdio.h>
#include <errno.h>


void error()
{
    printf("error\n");
    exit(1);
}

static char *read_file(char *path)
{
    // ファイルを開く
    FILE *fp = fopen(path, "r");
    if (!fp)
        error();

    // ファイルの長さを調べる
    if (fseek(fp, 0, SEEK_END) == -1)
        error();
    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1)
        error();

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

int main()
{
    printf("%s\n", read_file("Dockerfile"));
    return 0;
}

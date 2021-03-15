#include "hooligan.h"

Token *token;
Node *nodes[200];
Context *ctx;
FILE *output;

// 指定されたファイルの内容を返す
static char *read_file(char *path)
{
    // ファイルを開く
    FILE *fp = fopen(path, "r");
    if (!fp)
        error("cannot open %s: %s", path, strerror(errno));

    // ファイルの長さを調べる
    if (fseek(fp, 0, SEEK_END) == -1)
        error("%s: fseek: %s", path, strerror(errno));
    size_t size = ftell(fp);
    if (fseek(fp, 0, SEEK_SET) == -1)
        error("%s: fseek: %s", path, strerror(errno));

    // ファイル内容を読み込む
    char *buf = calloc(1, size + 2);
    fread(buf, size, 1, fp);

    // ファイルが必ず"\n\0"で終わっているようにする
    if (size == 0 || buf[size - 1] != '\n')
        buf[size++] = '\n';
    buf[size] = '\0';
    fclose(fp);
    return buf;
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        token = tokenize(read_file(argv[1]));
    }
    else
    {
        fprintf(stderr, "argument error\n");
        return 1;
    }

    FILE *file = fopen("tmp.s", "w");
    if (file == NULL)
    {
        printf("cannot open\n");
        exit(1);
    }
    gen_asm_intel(file);

    return 0;
}

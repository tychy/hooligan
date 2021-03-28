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
    if (argc == 1)
    {
        fprintf(stderr, "ファイル名を指定してください\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++)
    {
        token = NULL;
        char *p = read_file(argv[i]);
        char *p2 = calloc(1, 2 * strlen(p));
        memcpy(p2, p, strlen(p));
        preprocess(p2);
        token = tokenize(p2);
        char filename[4] = {'a' + i - 1, '.', 's', 0}; // a.s -> b.s -> c.s -> d.s
        output = fopen(filename, "w");
        if (output == NULL)
        {
            printf("cannot open\n");
            exit(1);
        }
        gen_asm_intel();
        fclose(output);
    }

    return 0;
}

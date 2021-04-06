#include "hooligan.h"

Token *token;
Node *nodes[500];
Context *ctx;
FILE *output;

// 指定されたファイルの内容を返す
static char *read_file(char *path)
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
char *preprocess_include(char *str, char *header)
{
    char *p = str;
    char *inc = "#include";
    while ((p = strstr(p, inc)) != NULL)
    {
        char *remain = p + strlen(inc);
        if (isspace(*remain))
        {
            remain++;
            if (*remain == '"')
            {
                remain++;
                if (strncmp(header, remain, strlen(header)) == 0)
                {
                    remain += strlen(header);
                    if (*remain == '"')
                    {
                        remain++;
                        while (*remain != '\n')
                        {
                            remain++;
                        }
                        remain++; //次の行の先頭
                        char *f = read_file(header);

                        size_t line_len = remain - p;
                        size_t f_len = strlen(f);
                        size_t prologue = p - str;
                        size_t epilogue = strlen(remain);
                        size_t size = prologue + f_len + epilogue;
                        // ... prologue
                        // #include "hooligan.h"
                        // ... epilogue

                        char *buf = calloc(1, 2 * size);
                        memcpy(buf, str, prologue);
                        memcpy(buf + prologue, f, f_len);
                        memcpy(buf + prologue + f_len, remain, epilogue);
                        str = buf;
                        p = buf + prologue + f_len;
                        continue;
                    }
                }
            }
        }
        p = remain;
    }
    return str;
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

        //p2 = preprocess_include(p2, "hooligan.h");
        preprocess(p2);

        //printf("%s", p2);

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

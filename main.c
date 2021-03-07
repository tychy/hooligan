#include "hooligan.h"

Token *token;
Vec *strings;
int label = 0;
Node *nodes[200];
Node *funcs[100];

// 指定されたファイルの内容を返す
char *read_file(char *path)
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

    printf(".intel_syntax noprefix\n");
    printf(".bss\n");
    program();
    int i = 0;
    int func_count = 0;
    while (nodes[i] != NULL)
    {
        if (nodes[i]->kind == ND_FUNCDEF)
        {
            funcs[func_count] = nodes[i];
            func_count++;
            i++;
            continue;
        }
        gen(nodes[i]);
        i++;
    }
    Vec *s = strings;
    printf(".data\n");
    while (s)
    {
        printf(".LC%d:\n", s->label);
        printf("  .string \"%.*s\"\n", s->length, s->p);
        s = s->next;
    }

    printf(".text\n");
    for (int j = 0; j < func_count; j++)
    {
        gen(funcs[j]);
    }
    return 0;
}

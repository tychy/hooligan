#include "hooligan.h"

Token *token;

int label = 0;
Node *nodes[100];

// 指定されたファイルの内容を返す
char *read_file(char *path) {
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
    if (argc != 2)
    {
        fprintf(stderr, "argument error\n");
        return 1;
    }

    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".bss\n");
    program();
    int i = 0;
    bool flag = true;
    while (nodes[i] != NULL)
    {
        if (flag && nodes[i]->kind != ND_GVARDEF)
        {
            printf(".text\n");
            flag = false;
        }
        else if (!flag && nodes[i]->kind == ND_FUNCDEF)
        {

            printf(".text\n");
        }
        gen(nodes[i]);
        i++;
    }
    return 0;
}

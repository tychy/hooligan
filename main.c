#include "hooligan.h"

Token *token;
Node *nodes[500];
Context *ctx;
FILE *output;

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

        p2 = preprocess(p2);
        printf("%s", p2);

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

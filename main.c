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

        pp_ctx = calloc(1, sizeof(PPContext));
        pp_ctx->macros = NULL;
        PPToken *pp_token = preprocess_directives(extract_dir(argv[i]), decompose_to_pp_token(read_file(argv[i])));

        token = tokenize(pp_token);
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

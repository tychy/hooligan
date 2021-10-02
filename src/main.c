#include "hooligan.h"

bool is_verbose;
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
    if (getenv("IS_VERBOSE") != NULL)
    {
        is_verbose = true;
    }
    else
    {
        is_verbose = false;
    }
    for (int i = 1; i < argc; i++)
    {
        token = NULL;
        char *path = argv[i];
        char *dirname = extract_dir(path);
        char *filename = extract_filename(path);
        char *p = read_file(path);
        if (is_verbose)
        {
            printf("%sのコンパイルを開始します\n", path);
        }

        pp_ctx = calloc(1, sizeof(PPContext));
        pp_ctx->macros = NULL;
        PPToken *pp_token = preprocess_directives(dirname, decompose_to_pp_token(p));

        token = tokenize(pp_token);
        char *output_filename = join_str(remove_extension(filename), ".s");
        output = fopen(output_filename, "w");

        if (output == NULL)
        {
            printf("cannot open\n");
            exit(1);
        }

        gen_asm_intel();

        fclose(output);
        if (is_verbose)
        {
            printf("\x1b[32mCompilation Succeded! output: %s\x1b[0m\n", output_filename);
        }
    }
    return 0;
}

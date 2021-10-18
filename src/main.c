#include "hooligan.h"
#include "inter_language.h"

Context *ctx;
FILE *output;
Option *opts;
int file_count;
char *files[20];

void analyze_arguments(int argc, char **argv)
{
    opts = calloc(1, sizeof(Option));
    file_count = 0;
    for (int i = 1; i < argc; i++)
    {
        char *arg = argv[i];
        if (*arg == '-')
        {
            if (*(arg + 1) == 'v')
            {
                opts->is_verbose = true;
            }
            else if (*(arg + 1) == 'O')
            {
                opts->optimize_level = 1;
            }
            else
            {
                printf("%c\n", *(arg + 1));
                error("不正なオプションです");
            }
        }
        else
        {
            files[file_count] = arg;
            file_count++;
        }
    }

    if (file_count == 0)
    {
        error("ファイル名を指定してください");
    }
}

int main(int argc, char **argv)
{
    analyze_arguments(argc, argv);

    for (int i = 0; i < file_count; i++)
    {
        char *path = files[i];
        char *dirname = extract_dir(path);
        char *filename = extract_filename(path);
        char *p = read_file(path);
        char *output_filename = join_str(remove_extension(filename), ".s");
        output = fopen(output_filename, "w");

        if (output == NULL)
        {
            printf("cannot open\n");
            exit(1);
        }
        if (opts->is_verbose)
        {
            printf("%sのコンパイルを開始します\n", path);
        }

        pp_ctx = calloc(1, sizeof(PPContext));
        pp_ctx->macros = NULL;
        ctx = calloc(1, sizeof(Context));
        ctx->scope = calloc(1, sizeof(Scope));

        PPToken *pp_token = preprocess_directives(dirname, decompose_to_pp_token(p));
        Token *token = tokenize(pp_token);

        Node **nodes = parse_program(token);
        ILSentence *ils = gen_asm_intel(nodes);
        if (opts->optimize_level != 0)
        {
            ils = optimize_inter_language(ils);
        }
        generate_intel_syntax_assembly(ils);

        fclose(output);
        if (opts->is_verbose)
        {
            printf("\x1b[32mCompilation Succeded! output: %s\x1b[0m\n", output_filename);
        }
    }
    return 0;
}

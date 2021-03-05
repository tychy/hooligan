#include "hooligan.h"

Token *token;

int label = 0;
Node *nodes[200];
Node *funcs[100];

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
    printf(".text\n");
    for (int j = 0; j < func_count; j++)
    {
        gen(funcs[j]);
    }
    return 0;
}

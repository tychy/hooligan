#include "hooligan.h"

Token *token;

int label = 0;
Node *nodes[100];

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "argument error\n");
        return 1;
    }

    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
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
        gen(nodes[i]);
        i++;
    }
    return 0;
}

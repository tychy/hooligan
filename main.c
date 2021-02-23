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
    program();
    int i = 0;
    while (nodes[i] != NULL)
    {
        gen(nodes[i]);
        i++;
    }
    return 0;
}

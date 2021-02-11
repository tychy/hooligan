#include "hooligan.h"

Token *token;

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
    printf("main:\n");
    printf("  mov rbp, rsp\n");
    program();
    int i = 0;
    while (nodes[i] != NULL)
    {
        gen(nodes[i]);
        i++;
        printf("  pop rax\n");
    }
    printf("  ret\n");
    return 0;
}

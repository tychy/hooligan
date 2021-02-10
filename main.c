#include "hooligan.h"

Token *token;

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
    Node *node = stmt();
    gen(node);
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}

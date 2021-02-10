#include "hooligan.h"

void gen(Node *node)
{
    if (node->kind == ND_NUM)
    {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    switch (node->kind)
    {
    case ND_ADD:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  add rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_SUB:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  sub rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_MUL:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  imul rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_DIV:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cqo\n");
        printf("  idiv rdi\n");
        printf("  push rax\n");
        break;
    case ND_EQUAL:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        break;
    case ND_NEQUAL:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
    case ND_GEQ:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setge al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
    case ND_LEQ:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
    case ND_GTH:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setge al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
    case ND_LTH:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");

        break;
    }
}

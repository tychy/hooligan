#include "hooligan.h"
void genl(Node *node)
{
    if (node->kind != ND_LVAR)
    {
        error("変数ではありません");
    }
    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
    return;
}
void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        genl(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        genl(node->lhs);
        gen(node->rhs);
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    case ND_IF:
        gen(node->lhs);
        if (node->rhs->kind == ND_ELSE)
        {
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lelse%d\n", label);
            gen(node->rhs->lhs);
            printf("  jmp .Lend%d\n", label);
            printf(".Lelse%d:\n", label);
            gen(node->rhs->rhs);
            printf(".Lend%d:\n", label);
            label++;
            return;
        }
        else
        {
            printf("  pop rax\n");
            printf("  cmp rax, 0\n");
            printf("  je .Lend%d\n", label);
            gen(node->rhs);
            printf("  jmp .Lend%d\n", label);
            printf(".Lend%d:\n", label);
            label++;
            return;
        }
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
        break;
    case ND_GEQ:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setge al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        break;
    case ND_LEQ:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        break;
    case ND_GTH:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  setg al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        break;
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

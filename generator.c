#include "hooligan.h"
void gen_for_init(Node *node, int lab)
{
    if (node->kind != ND_FORINIT)
    {
        error("for文として不適切です");
    }
    if (node->lhs != NULL)
    {
        gen(node->lhs);
    }
    printf(".Lforstart%d:\n", lab);
    if (node->rhs != NULL)
    {
        gen(node->rhs);
    }
    else
    {
        // 無条件でtrueとなるように
        printf("  push 1\n");
    }
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lforend%d\n", lab);
}

void gen_for_body(Node *node, int lab)
{
    if (node->kind != ND_FORBODY)
    {
        error("for文として不適切です");
    }
    gen(node->lhs);
    if (node->rhs != NULL)
    {
        gen(node->rhs);
    }
    printf("  jmp .Lforstart%d\n", lab);
}

void gen_for(Node *node, int lab)
{
    if (node->kind != ND_FOR)
    {
        error("for文ではありません");
    }
    gen_for_init(node->lhs, lab);
    gen_for_body(node->rhs, lab);
    printf(".Lforend%d:\n", lab);
}

void gen_while(Node *node, int lab)
{
    if (node->kind != ND_WHILE)
    {
        error("while文ではありません");
    }
    printf(".Lwhilestart%d:\n", lab);
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lwhileend%d\n", lab);
    gen(node->rhs);
    printf("  jmp .Lwhilestart%d\n", lab);
    printf(".Lwhileend%d:\n", lab);
}

void gen_else(Node *node, int lab)
{
    if (node->kind != ND_ELSE)
    {
        error("else文ではありません");
    }
    gen(node->lhs);
    printf("  jmp .Lend%d\n", lab);
    printf(".Lelse%d:\n", lab);
    gen(node->rhs);
    printf(".Lend%d:\n", lab);
}

void gen_if(Node *node, int lab)
{
    if (node->kind != ND_IF)
    {
        error("if文ではありません");
    }
    gen(node->lhs);
    if (node->rhs->kind == ND_ELSE)
    {
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lelse%d\n", lab);
        gen_else(node->rhs, lab);
    }
    else
    {
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", lab);
        gen(node->rhs);
        printf("  jmp .Lend%d\n", lab);
        printf(".Lend%d:\n", lab);
        return;
    }
}

void gen_function(Node *node)
{
    if (node->kind != ND_FUNC)
    {
        error("関数ではありません");
    }
    Node *arg = node->rhs;
    Node *first_arg = NULL;
    int count = 1;
    while (arg != NULL)
    {
        if (arg->kind != ND_ARG)
        {
            error("引数ではありません");
        }
        // 第一引数のレジスタRDIは計算で使われるため最後にpopしなければならない
        if (count == 1)
        {
            first_arg = arg;
            arg = arg->rhs;
            count++;
            continue;
        }
        gen(arg->lhs);
        switch (count)
        {
        case 2:
            printf("  pop rsi\n");
            break;
        case 3:
            printf("  pop rdx\n");
            break;
        case 4:
            printf("  pop rcx\n");
            break;
        case 5:
            printf("  pop r8\n");
            break;
        case 6:
            printf("  pop r9\n");
            break;
        default:
            error("引数の数が多すぎます");
            break;
        }
        arg = arg->rhs;
        count++;
    }
    if (first_arg != NULL)
    {
        gen(first_arg->lhs);
        printf("  pop rdi\n");
    }
    printf("  call %.*s\n", node->length, node->name);
    printf("  push rax\n");
}

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

void gen_function_def(Node *node)
{
    if (node->kind != ND_FUNCDEF)
    {
        error("関数定義ではありません");
    }
    printf("%.*s:\n", node->length, node->name);
    // プロローグ
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    // 第1〜6引数をローカル変数の領域に書き出す
    int count = 1;
    Node *arg = node->lhs;
    while (arg != NULL)
    {
        genl(arg);
        printf("  pop rax\n");
        switch (count)
        {
        case 1:
            printf("  mov [rax], rdi\n");
            break;
        case 2:
            printf("  mov [rax], rsi\n");
            break;
        case 3:
            printf("  mov [rax], rdx\n");
            break;
        case 4:
            printf("  mov [rax], rcx\n");
            break;
        case 5:
            printf("  mov [rax], r8\n");
            break;
        case 6:
            printf("  mov [rax], r9\n");
            break;
        default:
            error("引数の数が多すぎます");
        }
        count++;
        arg = arg->lhs;
    }

    gen(node->rhs);
    printf("  pop rax\n");

    // エピローグ
    // ここに書くと多分returnなしで戻り値を指定できるようになってしまう、どうすべきか
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
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
        label++;
        gen_if(node, label);
        return;
    case ND_FOR:
        label++;
        gen_for(node, label);
        return;
    case ND_BLOCK:
        if (node->lhs == NULL)
            return;
        gen(node->lhs);
        if (node->rhs->lhs == NULL)
            return;
        printf("  pop rax\n");
        gen(node->rhs);
        return;
    case ND_WHILE:
        label++;
        gen_while(node, label);
        return;
    case ND_FUNC:
        gen_function(node);
        return;
    case ND_FUNCDEF:
        gen_function_def(node);
        return;
    case ND_ADDR:
        genl(node->lhs);
        return;
    case ND_DEREF:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
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

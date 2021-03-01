#include "hooligan.h"

void gen_for(Node *node, int lab)
{
    if (node->kind != ND_FOR)
        error("for文ではありません");

    if (node->init != NULL)
        gen(node->init);
    printf(".Lforstart%d:\n", lab);

    if (node->condition != NULL)
        gen(node->condition);
    else
        printf("  push 1\n"); // 無条件でtrueとなるように
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lforend%d\n", lab);

    gen(node->body);

    if (node->on_end != NULL)
        gen(node->on_end);
    printf("  jmp .Lforstart%d\n", lab);
    printf(".Lforend%d:\n", lab);
}

void gen_while(Node *node, int lab)
{
    if (node->kind != ND_WHILE)
        error("while文ではありません");
    printf(".Lwhilestart%d:\n", lab);
    gen(node->condition);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    printf("  je .Lwhileend%d\n", lab);
    gen(node->body);
    printf("  jmp .Lwhilestart%d\n", lab);
    printf(".Lwhileend%d:\n", lab);
}

void gen_if(Node *node, int lab)
{
    if (node->kind != ND_IF)
        error("if文ではありません");
    bool else_exist = node->on_else;
    gen(node->condition);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    if (else_exist)
        printf("  je .Lelse%d\n", lab);
    else
        printf("  je .Lend%d\n", lab);
    gen(node->body);
    if (else_exist)
    {
        printf("  jmp .Lend%d\n", lab);
        printf(".Lelse%d:\n", lab);
        gen(node->on_else);
    }
    printf(".Lend%d:\n", lab);
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

void gen_global_var(Node *node)
{
    if (node->kind != ND_GVAR)
    {
        error("グローバル変数ではありません");
    }
    // TODO(yokotsuka): 現状グローバル変数はint型のみのため分岐しない
    printf("  mov eax, dword ptr %.*s[rip]\n", node->length, node->name);
}

void gen_global_var_def(Node *node)
{
    if (node->kind != ND_GVARDEF)
    {
        error("グローバル変数定義ではありません");
    }
    printf("%.*s:\n", node->length, node->name);
    printf("  .zero  4\n");
}

void genl(Node *node)
{
    if (node->kind == ND_DEREF)
    {
        gen(node->lhs);
        return;
    }
    if (node->kind == ND_LVAR)
    {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", node->offset);
        printf("  push rax\n");
    }
    else if (node->kind == ND_GVAR)
    {
        printf("  mov rax, offset flat:%.*s\n", node->length, node->name);
        printf("  push rax\n");
    }
    else
        error("変数ではありません");
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
    printf("  sub rsp, %d\n", node->args_region_size);

    // 第1〜6引数をローカル変数の領域に書き出す
    int count = 1;
    Node *arg = node->lhs;
    while (arg != NULL)
    {
        genl(arg);
        printf("  pop rax\n");
        switch (count)
        {
            // TODO 関数の引数はintのみという前提に基づいている
        case 1:
            printf("  mov [rax], edi\n");
            break;
        case 2:
            printf("  mov [rax], esi\n");
            break;
        case 3:
            printf("  mov [rax], edx\n");
            break;
        case 4:
            printf("  mov [rax], ecx\n");
            break;
        case 5:
            printf("  mov [rax], r8d\n");
            break;
        case 6:
            printf("  mov [rax], r9d\n");
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
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        printf("  pop rax\n");
        if (node->ty->ty == INT)
            printf("  mov eax, [rax]\n");
        else
            printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        genl(node->lhs);
        gen(node->rhs);
        printf("  pop rdi\n");
        printf("  pop rax\n");
        if (node->ty->ty == INT)
            printf("  mov [rax], edi\n");
        else
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
    case ND_GVAR:
        gen_global_var(node);
        return;
    case ND_GVARDEF:
        gen_global_var_def(node);
        return;
    case ND_ADDR:
        genl(node->lhs);
        return;
    case ND_DEREF:
        gen(node->lhs);
        printf("  pop rax\n");
        if (node->ty->ty == INT)
            printf("  mov eax, [rax]\n");
        else
            printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ADD:
    case ND_SUB:
        gen(node->lhs);
        gen(node->rhs);
        if (node->ty->ty == INT)
        {
            printf("  pop rdi\n");
            printf("  pop rax\n");
            if (node->kind == ND_ADD)
                printf("  add rax, rdi\n");
            else
                printf("  sub rax, rdi\n");
        }
        else
        {
            printf("  pop rdi\n");
            printf("  pop rax\n");
            int size = calc_bytes(node->ty->ptr_to);
            if (node->lhs->ty->ty == INT)
            {
                printf("  imul rax, %d\n", size);
            }
            else if (node->rhs->ty->ty == INT)
            {
                printf("  imul rdi, %d\n", size);
            }
            else
            {
                error("式にはintが必要です");
            }

            if (node->kind == ND_ADD)
                printf("  add rax, rdi\n");
            else
                printf("  sub rax, rdi\n");
        }
        printf("  push rax\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    switch (node->kind)
    {
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

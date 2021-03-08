#include "hooligan.h"
static char *reg32[6] = {
    "edi",
    "esi",
    "edx",
    "ecx",
    "r8d",
    "r9d",
};
static char *reg64[6] = {
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8",
    "r9",
};

static void gen_for(Node *node, int lab)
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

static void gen_while(Node *node, int lab)
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

static void gen_if(Node *node, int lab)
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

static void gen_function(Node *node)
{
    if (node->kind != ND_FUNC)
    {
        error("関数ではありません");
    }
    Node *arg = node->rhs;
    Node *first_arg = NULL;
    int count = 0;
    while (arg != NULL)
    {
        if (arg->kind != ND_ARG)
        {
            error("引数ではありません");
        }
        // 第一引数のレジスタRDIは計算で使われるため最後にpopしなければならない
        if (count == 0)
        {
            first_arg = arg;
            arg = arg->rhs;
            count++;
            continue;
        }
        gen(arg->lhs);

        if (count < 6)
        {
            printf("  pop  %s\n", reg64[count]);
        }
        else
        {

            error("引数の数が多すぎます");
        }

        arg = arg->rhs;
        count++;
    }
    if (first_arg != NULL)
    {
        gen(first_arg->lhs);
        printf("  pop %s\n", reg64[0]);
    }
    printf("  call %.*s\n", node->length, node->name);
    printf("  push rax\n");
}

static void gen_global_var_def(Node *node)
{
    if (node->kind != ND_GVARDEF)
    {
        error("グローバル変数定義ではありません");
    }
    printf("%.*s:\n", node->length, node->name);
    printf("  .zero  %d\n", calc_bytes(node->ty));
}

static void gen_var(Node *node)
{
    if (node->kind == ND_DEREF)
    {
        gen(node->lhs);
        return;
    }
    if (node->kind != ND_VAR)
        error("変数ではありません");

    if (node->is_local)
    {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n", node->offset);
        printf("  push rax\n");
    }
    else
    {
        printf("  lea rax, %.*s\n", node->length, node->name);
        printf("  push rax\n");
    }
    return;
}

static void gen_function_def(Node *node)
{
    if (node->kind != ND_FUNCDEF)
    {
        error("関数定義ではありません");
    }

    printf(".globl %.*s\n", node->length, node->name);
    printf("%.*s:\n", node->length, node->name);
    // プロローグ
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", node->args_region_size);

    // 第1〜6引数をローカル変数の領域に書き出す
    int count = 0;
    Node *arg = node->lhs;
    while (arg != NULL)
    {
        gen_var(arg);
        printf("  pop rax\n");
        if (count < 6)
        {
            char *reg;
            if (is_int_or_char(arg->ty))
                reg = reg32[count];
            else
                reg = reg64[count];

            printf("  mov [rax], %s\n", reg);
        }
        else
        {

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

void gen_assign(Node *node)
{
    if (node->kind != ND_ASSIGN)
    {
        error("代入式ではありません");
    }
    gen_var(node->lhs);
    gen(node->rhs);
    printf("  pop rdi\n");
    printf("  pop rax\n");
    if (is_int(node->ty))
        printf("  mov [rax], edi\n");
    else if (is_char(node->ty))
    {
        printf("  mov [rax], dil\n");
    }
    else
        printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
}

void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_VAR:
        gen_var(node);
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        printf("  pop rax\n");
        if (is_int(node->ty))
            printf("  mov eax, [rax]\n");
        else if (is_char(node->ty))
        {
            printf("  movsx eax, BYTE PTR [rax]\n");
        }
        else
            printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        gen_assign(node);
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
    case ND_GVARDEF:
        gen_global_var_def(node);
        return;
    case ND_STRING:
        printf("  push offset .LC%d\n", node->strlabel);
        return;
    case ND_ADDR:
        gen_var(node->lhs);
        return;
    case ND_DEREF:
        gen(node->lhs);
        printf("  pop rax\n");
        if (is_int(node->ty))
            printf("  mov eax, [rax]\n");
        else if (is_char(node->ty))
        {
            printf("  movsx eax, BYTE PTR [rax]\n");
        }
        else
            printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ADD:
    case ND_SUB:
        gen(node->lhs);
        gen(node->rhs);
        if (is_int_or_char(node->ty))
        {
            printf("  pop rdi\n");
            printf("  pop rax\n");
            if (node->kind == ND_ADD)
                printf("  add eax, edi\n");
            else
                printf("  sub eax, edi\n");
        }
        else
        {
            printf("  pop rdi\n");
            printf("  pop rax\n");
            int size = calc_bytes(node->ty->ptr_to);
            if (is_int_or_char(node->lhs->ty))
            {
                printf("  imul rax, %d\n", size);
            }
            else if (is_int_or_char(node->rhs->ty))
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
        printf("  cdq\n");
        printf("  idiv edi\n");
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

#include "hooligan.h"
static void gen(Node *node);

static char *reg32[9] = {
    "edi",
    "esi",
    "edx",
    "ecx",
    "r8d",
    "r9d",
    "eax",
    "ebp",
    "esp",
};
static char *reg64[9] = {
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8",
    "r9",
    "rax",
    "rbp",
    "rsp",
};

// TODO 写経したけどなんでうごくのかわからない
__attribute__((format(printf, 1, 2))) static void println(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(output, fmt, ap);
    va_end(ap);
    fprintf(output, "\n");
}

int depth;
static void push(RegisterName r)
{
    println("  push %s", reg64[r]);
    depth++;
}

static void push_val(int val)
{
    println("  push %d", val);
    depth++;
}

static void push_str_addr(int label)
{
    println("  push offset .LC%d", label);
    depth++;
}

static void pop(RegisterName r)
{
    println("  pop %s", reg64[r]);
    depth--;
}

static void gen_for(Node *node)
{
    if (node->kind != ND_FOR)
        error("for文ではありません");
    int lab = node->loop_label;
    if (node->init != NULL)
        gen(node->init);
    println(".L.Start%d:", lab);

    if (node->condition != NULL)
        gen(node->condition);
    else
        push_val(1); // 無条件でtrueとなるように
    pop(RG_RAX);
    println("  cmp rax, 0");
    println("  je .L.End%d", lab);

    gen(node->body);

    if (node->on_end != NULL)
        gen(node->on_end);
    println("  jmp .L.Start%d", lab);
    println(".L.End%d:", lab);
}

static void gen_while(Node *node)
{
    if (node->kind != ND_WHILE)
        error("while文ではありません");
    int lab = node->loop_label;
    println(".L.Start%d:", lab);
    gen(node->condition);
    pop(RG_RAX);
    println("  cmp rax, 0");
    println("  je .L.End%d", lab);
    gen(node->body);
    println("  jmp .L.Start%d", lab);
    println(".L.End%d:", lab);
}

static void gen_if(Node *node)
{
    if (node->kind != ND_IF)
        error("if文ではありません");
    bool else_exist = node->on_else;
    int lab = node->cond_label;
    gen(node->condition);
    pop(RG_RAX);
    println("  cmp rax, 0");
    if (else_exist)
        println("  je .Lelse%d", lab);
    else
        println("  je .Lend%d", lab);
    gen(node->body);
    if (else_exist)
    {
        println("  jmp .Lend%d", lab);
        println(".Lelse%d:", lab);
        gen(node->on_else);
    }
    println(".Lend%d:", lab);
}

static void gen_function(Node *node) // gen_function_callとかのほうがいい気がする
{
    if (node->kind != ND_FUNC)
    {
        error("関数ではありません");
    }
    Node *arg = node->next;
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
            arg = arg->next;
            count++;
            continue;
        }
        gen(arg->child);

        if (count < 6)
        {
            pop(count);
        }
        else
        {
            error("引数の数が多すぎます");
        }

        arg = arg->next;
        count++;
    }
    if (first_arg != NULL)
    {
        gen(first_arg->child);
        pop(0);
    }
    if (depth % 2 == 0)
    {
        println("  sub rsp, 8");
    }
    println("  mov al, 0");
    if (node->is_static)
    {
        println("  call L%.*s", node->length, node->name);
    }
    else
    {
        println("  call %.*s", node->length, node->name);
    }
    if (depth % 2 == 0)
    {
        println("  add rsp, 8");
    }
    push(RG_RAX);
}

static void gen_global_var_def(Node *node)
{
    if (node->kind != ND_GVARDEF)
    {
        error("グローバル変数定義ではありません");
    }
    if (node->is_static)
    {

        println("L%.*s:", node->length, node->name);
    }
    else
    {
        println("%.*s:", node->length, node->name);
    }
    println("  .zero  %d", calc_bytes(node->ty));
}

static void gen_addr(Node *node)
{
    switch (node->kind)
    {
    case ND_DEREF:
        gen(node->child);
        return;
    case ND_VAR:
        if (node->is_local && node->is_static)
        {
            println("  lea rax, L%.*s.%d", node->length, node->name, node->scope_label);
            push(RG_RAX);
        }
        else if (node->is_local)
        {
            println("  mov rax, rbp");
            println("  sub rax, %d", node->offset);
            push(RG_RAX);
        }
        else if (node->is_static)
        {
            println("  lea rax, L%.*s", node->length, node->name);
            push(RG_RAX);
        }
        else
        {
            println("  lea rax, %.*s", node->length, node->name);
            push(RG_RAX);
        }
        return;
    case ND_MEMBER:
        gen_addr(node->child);
        pop(RG_RAX);
        println("  add rax, %d", node->member->offset);
        push(RG_RAX);
        return;

    default:
        error("アドレスが計算できません");
    }
}

static void gen_function_def(Node *node) // こっちがgen_functionという名前がいい気がする
{
    if (node->kind != ND_FUNCDEF)
    {
        error("関数定義ではありません");
    }

    if (node->is_static)
    {
        println("L%.*s:", node->length, node->name);
    }
    else
    {
        println(".globl %.*s", node->length, node->name);
        println("%.*s:", node->length, node->name);
    }
    // プロローグ
    push(RG_RBP);
    println("  mov rbp, rsp");
    println("  sub rsp, %d", node->args_region_size);

    // 第1〜6引数をローカル変数の領域に書き出す
    int count = 0;
    Node *arg = node->lhs;
    while (arg != NULL)
    {
        gen_addr(arg);
        pop(RG_RAX);
        if (count < 6)
        {
            char *reg;
            if (is_int_or_char(arg->ty))
                reg = reg32[count];
            else
                reg = reg64[count];

            println("  mov [rax], %s", reg);
        }
        else
        {

            error("引数の数が多すぎます");
        }
        count++;
        arg = arg->lhs;
    }

    gen(node->rhs);
    pop(RG_RAX);

    // エピローグ
    // ここに書くと多分returnなしで戻り値を指定できるようになってしまう、どうすべきか
    println("  mov rsp, rbp");
    pop(RG_RBP);
    println("  ret");
}

static void gen_assign(Node *node)
{
    if (node->kind != ND_ASSIGN)
    {
        error("代入式ではありません");
    }
    if (node->rhs && node->rhs->kind == ND_INIT)
    {
        if (is_int_or_char(node->lhs->ty))
        {
            error("ポインタ型が必要です");
        }
        Node *cur = node->rhs;
        int counter = 0;
        while (cur)
        {
            gen_addr(node->lhs);
            gen(cur->child);
            pop(RG_RDI);
            pop(RG_RAX);
            println("  add rax, %d", calc_bytes(node->lhs->ty->ptr_to) * counter);
            if (is_int(node->lhs->ty->ptr_to))
                println("  mov [rax], edi");
            else if (is_char(node->lhs->ty->ptr_to))
            {
                println("  mov [rax], dil");
            }
            else
                println("  mov [rax], rdi");
            cur = cur->next;
            counter++;
        }
        return;
    }

    gen_addr(node->lhs);
    gen(node->rhs);
    pop(RG_RDI);
    pop(RG_RAX);
    if (is_int(node->ty))
        println("  mov [rax], edi");
    else if (is_char(node->ty))
    {
        println("  mov [rax], dil");
    }
    else
        println("  mov [rax], rdi");
    push(RG_RDI);
}

void gen_block(Node *node)
{
    if (node->kind != ND_BLOCK)
    {
        error("ブロックではありません");
    }
    Node *cur = node->statements;
    while (cur)
    {
        gen(cur);
        cur = cur->next_stmt;
    }
}

void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        push_val(node->val);
        return;
    case ND_NOT:
        gen(node->child);
        pop(RG_RAX);
        println("  cmp rax, 0");
        println("  sete al");
        println("  movzb rax, al");
        push(RG_RAX);
        return;
    case ND_VAR:
        gen_addr(node);
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        pop(RG_RAX);
        if (is_int(node->ty))
            println("  mov eax, [rax]");
        else if (is_char(node->ty))
        {
            println("  movsx eax, BYTE PTR [rax]");
        }
        else
            println("  mov rax, [rax]");
        push(RG_RAX);
        return;
    case ND_ASSIGN:
        gen_assign(node);
        return;
    case ND_RETURN:
        gen(node->child);
        pop(RG_RAX);
        println("  mov rsp, rbp");
        pop(RG_RBP);
        println("  ret");
        return;
    case ND_IF:
        gen_if(node);
        return;
    case ND_FOR:
        gen_for(node);
        return;
    case ND_BLOCK:
        gen_block(node);
        return;
    case ND_WHILE:
        gen_while(node);
        return;
    case ND_SWITCH:
        gen(node->condition);
        pop(RG_RAX);
        for (Node *c = node->next_case; c; c = c->next_case)
        {
            println("  cmp eax, %d", c->val);
            println("  je .L.Case%d", c->case_label);
        }
        if (node->default_case)
        {
            println("  jmp .L.Case%d", node->default_case->case_label);
        }
        println("  jmp .L.End%d", node->break_to);
        gen(node->child);
        println(".L.End%d:", node->break_to);
        return;
    case ND_CASE:
        println(".L.Case%d:", node->case_label);
        gen(node->child);
        return;
    case ND_BREAK:
        println("  jmp .L.End%d", node->loop_label);
        return;
    case ND_CONTINUE:
        println("  jmp .L.Start%d", node->loop_label);
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
        push_str_addr(node->strlabel);
        return;
    case ND_MEMBER:
        gen_addr(node);
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        pop(RG_RAX);
        if (is_int(node->member->ty))
            println("  mov eax, [rax]");
        else if (is_char(node->member->ty))
        {
            println("  movsx eax, BYTE PTR [rax]");
        }
        else
            println("  mov rax, [rax]");
        push(RG_RAX);
        return;
    case ND_ADDR:
        gen_addr(node->child);
        return;
    case ND_DEREF:
        gen(node->child);
        pop(RG_RAX);
        if (is_int(node->ty))
            println("  mov eax, [rax]");
        else if (is_char(node->ty))
        {
            println("  movsx eax, BYTE PTR [rax]");
        }
        else
            println("  mov rax, [rax]");
        push(RG_RAX);
        return;
    case ND_POSTINC:
        gen(node->lhs); // インクリメント前の値がpushされる
        gen(node->rhs); // インクリメント後の値がpushされる
        pop(RG_RAX);    // インクリメント後の値がpopされる
        // スタックトップはインクリメント前の値
        return;
    case ND_NOP:
        println("  nop");
        return;
    case ND_ADD:
    case ND_SUB:
        gen(node->lhs);
        gen(node->rhs);
        if (is_int_or_char(node->ty))
        {
            pop(RG_RDI);
            pop(RG_RAX);
            if (node->kind == ND_ADD)
                println("  add eax, edi");
            else
                println("  sub eax, edi");
        }
        else
        {
            pop(RG_RDI);
            pop(RG_RAX);
            int size = calc_bytes(node->ty->ptr_to);
            if (is_int_or_char(node->lhs->ty))
            {
                println("  imul rax, %d", size);
            }
            else if (is_int_or_char(node->rhs->ty))
            {
                println("  imul rdi, %d", size);
            }
            else
            {
                error("式にはintが必要です");
            }

            if (node->kind == ND_ADD)
                println("  add rax, rdi");
            else
                println("  sub rax, rdi");
        }
        push(RG_RAX);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    switch (node->kind)
    {
    case ND_MUL:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  imul rax, rdi");
        push(RG_RAX);
        break;
    case ND_DIV:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cdq");
        println("  idiv edi");
        push(RG_RAX);
        break;
    case ND_MOD:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cdq");
        println("  idiv edi");
        push(RG_RDX);
        break;
    case ND_EQUAL:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  sete al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_NEQUAL:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  setne al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_GEQ:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  setge al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_LEQ:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  setle al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_GTH:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  setg al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_LTH:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  cmp rax, rdi");
        println("  setl al");
        println("  movzb rax, al");
        push(RG_RAX);
        break;
    case ND_AND:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  and rax, rdi");
        push(RG_RAX);
        break;
    case ND_OR:
        pop(RG_RDI);
        pop(RG_RAX);
        println("  or rax, rdi");
        push(RG_RAX);
        break;
    }
}

void gen_asm_intel()
{
    Node *funcs[100];
    println(".intel_syntax noprefix");
    println(".data");
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
    String *s = ctx->strings;
    Var *sv = ctx->statics;
    while (sv)
    {
        println("L%.*s.%d:", sv->length, sv->name, sv->label);
        switch (sv->ty->ty)
        {
        case CHAR:
            println("  .byte %d", sv->init_val);
            break;
        case INT:
            println("  .long %d", sv->init_val);
            break;
        case PTR:
            println("  .quad %d", sv->init_val);
            break;
        case ARRAY:
        case STRUCT:
            println("  .zero  %d", calc_bytes(sv->ty));
            break;
        default:
            error("型がサポートされていません");
        }

        sv = sv->next;
    }

    while (s)
    {
        println(".LC%d:", s->label);
        println("  .string \"%.*s\"", s->length, s->p);
        s = s->next;
    }

    println(".text");
    for (int j = 0; j < func_count; j++)
    {
        gen(funcs[j]);
    }

    for (int i = 0; i < 200; i++)
    {
        nodes[i] = NULL;
    }
}

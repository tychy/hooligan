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

void println(char *fmt)
{
    fprintf(output, fmt);
    fprintf(output, "\n");
}

void println1(char *fmt, char *v1)
{
    fprintf(output, fmt, v1);
    fprintf(output, "\n");
}

void println2(char *fmt, char *v1, char *v2)
{
    fprintf(output, fmt, v1, v2);
    fprintf(output, "\n");
}

void println3(char *fmt, char *v1, char *v2, char *v3)
{
    fprintf(output, fmt, v1, v2, v3);
    fprintf(output, "\n");
}

int depth;
static void push(RegisterName r)
{
    println1("  push %s", reg64[r]);
    depth++;
}

static void push_val(int val)
{
    println1("  push %d", val);
    depth++;
}

static void push_str_addr(int label)
{
    println1("  push offset .LC%d", label);
    depth++;
}

static void pop(RegisterName r)
{
    println1("  pop %s", reg64[r]);
    depth--;
}

static void gen_for(Node *node)
{
    if (node->kind != ND_FOR)
        error("for文ではありません");
    int lab = node->loop_label;
    if (node->init != NULL)
        gen(node->init);
    println1("  jmp .L.Cond%d", lab);
    println1(".L.Start%d:", lab);
    gen(node->body);
    println1(".L.OnEnd%d:", lab);
    if (node->on_end != NULL)
        gen(node->on_end);
    println1(".L.Cond%d:", lab);
    if (node->condition != NULL)
        gen(node->condition);
    else
        push_val(1); // 無条件でtrueとなるように
    pop(RG_RAX);
    println("  cmp rax, 0");
    println1("  je .L.End%d", lab);
    println1("  jmp .L.Start%d", lab);
    println1(".L.End%d:", lab);
}

static void gen_while(Node *node)
{
    if (node->kind != ND_WHILE)
        error("while文ではありません");
    int lab = node->loop_label;
    println1("  jmp .L.Cond%d", lab);
    println1(".L.Start%d:", lab);
    gen(node->body);
    println1(".L.OnEnd%d:", lab);
    println1(".L.Cond%d:", lab);
    gen(node->condition);
    pop(RG_RAX);
    println("  cmp rax, 0");
    println1("  je .L.End%d", lab);
    println1("  jmp .L.Start%d", lab);
    println1(".L.End%d:", lab);
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
        println1("  je .Lelse%d", lab);
    else
        println1("  je .Lend%d", lab);
    gen(node->body);
    if (else_exist)
    {
        println1("  jmp .Lend%d", lab);
        println1(".Lelse%d:", lab);
        gen(node->on_else);
    }
    println1(".Lend%d:", lab);
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
        gen(arg->child);

        if (count >= 6)
        {
            error("引数の数が多すぎます");
        }

        arg = arg->next;
        count++;
    }
    while (count > 0)
    {
        count--;
        pop(count);
    }
    if (depth % 2 == 0)
    {
        println("  sub rsp, 8");
    }
    println("  mov al, 0");
    if (node->is_static)
    {
        println2("  call L%.*s", node->length, node->name);
    }
    else
    {
        println2("  call %.*s", node->length, node->name);
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

        println2("L%.*s:", node->length, node->name);
    }
    else
    {
        println2(".globl %.*s", node->length, node->name);
        println2("%.*s:", node->length, node->name);
    }
    if (node->gvar_init)
    {

        if (is_int_or_char(node->ty))
        {
            println1("  .long %d", node->gvar_init->val);
        }
        else if (is_char(node->ty->ptr_to))
        {
            println1("  .quad .LC%d", node->gvar_init->strlabel);
        }
        else
        {
            Node *cur = node->gvar_init;
            int counter = 0;
            while (cur)
            {
                if (cur->child->kind == ND_STRING)
                {
                    println1("  .quad .LC%d", cur->child->strlabel);
                }
                else
                {
                    println1("  .long %d", cur->child->val);
                }
                cur = cur->next;
                counter++;
            }
            int remain_size = (node->ty->array_size - counter) * calc_bytes(node->ty->ptr_to);
            if (remain_size)
            {
                println1("  .zero %d", remain_size);
            }
            return;
        }
    }
    else
    {
        println1("  .zero  %d", calc_bytes(node->ty));
    }
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
            println3("  lea rax, L%.*s.%d", node->length, node->name, node->scope_label);
            push(RG_RAX);
        }
        else if (node->is_local)
        {
            println("  mov rax, rbp");
            println1("  sub rax, %d", node->offset);
            push(RG_RAX);
        }
        else if (node->is_static)
        {
            println2("  lea rax, L%.*s", node->length, node->name);
            push(RG_RAX);
        }
        else
        {
            println2("  lea rax, %.*s", node->length, node->name);
            push(RG_RAX);
        }
        return;
    case ND_MEMBER:
        gen_addr(node->child);
        pop(RG_RAX);
        println1("  add rax, %d", node->member->offset);
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
        println2("L%.*s:", node->length, node->name);
    }
    else
    {
        println2(".globl %.*s", node->length, node->name);
        println2("%.*s:", node->length, node->name);
    }
    // プロローグ
    push(RG_RBP);
    println("  mov rbp, rsp");
    println1("  sub rsp, %d", 16 * (node->args_region_size / 16 + 1));

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

            println1("  mov [rax], %s", reg);
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
            println1("  add rax, %d", calc_bytes(node->lhs->ty->ptr_to) * counter);
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

static void gen(Node *node)
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
            println1("  cmp eax, %d", c->val);
            println1("  je .L.Case%d", c->case_label);
        }
        if (node->default_case)
        {
            println1("  jmp .L.Case%d", node->default_case->case_label);
        }
        println1("  jmp .L.End%d", node->break_to);
        gen(node->child);
        println1(".L.End%d:", node->break_to);
        return;
    case ND_CASE:
        println1(".L.Case%d:", node->case_label);
        gen(node->child);
        return;
    case ND_BREAK:
        println1("  jmp .L.End%d", node->loop_label);
        return;
    case ND_CONTINUE:
        println1("  jmp .L.OnEnd%d", node->loop_label);
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
    case ND_POSTDEC:
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
                println1("  imul rax, %d", size);
            }
            else if (is_int_or_char(node->rhs->ty))
            {
                println1("  imul rdi, %d", size);
            }
            else if (is_ptr(node->rhs->ty) && is_ptr(node->lhs->ty) && node->kind == ND_SUB)
            {
                println("  sub rax, rdi");
                println("  cqo");
                println1("  mov rdi, %d", size);
                println("  idiv rdi");
                push(RG_RAX);
                return;
            }
            else
            {
                error("適切でない演算です");
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
        println3("L%.*s.%d:", sv->length, sv->name, sv->label);
        switch (sv->ty->ty)
        {
        case CHAR:
            println1("  .byte %d", sv->init_val);
            break;
        case INT:
            println1("  .long %d", sv->init_val);
            break;
        case PTR:
            println1("  .quad %d", sv->init_val);
            break;
        case ARRAY:
        case STRUCT:
            println1("  .zero  %d", calc_bytes(sv->ty));
            break;
        default:
            error("型がサポートされていません");
        }

        sv = sv->next;
    }

    while (s)
    {
        println1(".LC%d:", s->label);
        println2("  .string \"%.*s\"", s->length, s->p);
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

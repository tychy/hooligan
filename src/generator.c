#include "hooligan.h"

#include "generator/il_constructor.c"
#include "generator/register.c"
#include "generator/shorthand.c"
#include "generator/builtin.c"

static void gen(Node *node);

static void gen_float(Node *node)
{
    if (node->kind != ND_FLOAT)
    {
        error("floatではありません");
    }
    push_str_addr(node->id); // 流用しているので関数名を変えるべき
    pop(ILRG_RAX);
    new_il_sentence_raw("  movss xmm0, DWORD PTR [rax]");
    new_il_sentence_raw("  mov eax, [rax]");
    push(ILRG_RAX);
    new_il_sentence_raw_to_data(".LC%d:", node->id);
    new_il_sentence_raw_to_data("   .long %u", f2bin(node->f_val));
}

static void gen_logical_operator(Node *node)
{
    gen(node->lhs);
    pop(ILRG_RAX);
    new_il_sentence_raw("  cmp rax, 0");
    if (node->kind == ND_AND)
    {
        new_il_sentence_raw("  je .L.LOGICALOPERATOR%d", node->id);
    }
    else if (node->kind == ND_OR)
    {
        new_il_sentence_raw("  jne .L.LOGICALOPERATOR%d", node->id);
    }
    new_il_sentence_raw("  setne al");
    push(ILRG_RAX);
    gen(node->rhs);
    pop(ILRG_RDI);
    pop(ILRG_RAX);
    new_il_sentence_raw("  cmp rdi, 0");
    new_il_sentence_raw("  setne dil");
    if (node->kind == ND_AND)
    {
        new_il_sentence_raw("  and al, dil");
    }
    else if (node->kind == ND_OR)
    {
        new_il_sentence_raw("  or al, dil");
    }
    new_il_sentence_raw("  movzb rax, al");
    new_il_sentence_raw("  .L.LOGICALOPERATOR%d:", node->id);
    push(ILRG_RAX);
}

static void gen_loop(Node *node)
{
    int lab = node->id;
    if (node->init != NULL)
        gen(node->init);
    new_il_sentence_raw("  jmp .L.Cond%d", lab);
    new_il_sentence_raw(".L.Start%d:", lab);
    gen(node->body);
    new_il_sentence_raw(".L.OnEnd%d:", lab);
    if (node->on_end != NULL)
        gen(node->on_end);
    new_il_sentence_raw(".L.Cond%d:", lab);
    if (node->condition != NULL)
        gen(node->condition);
    else
        push_val(1); // 無条件でtrueとなるように
    pop(ILRG_RAX);
    new_il_sentence_raw("  cmp rax, 0");
    new_il_sentence_raw("  je .L.End%d", lab);
    new_il_sentence_raw("  jmp .L.Start%d", lab);
    new_il_sentence_raw(".L.End%d:", lab);
}

static void gen_if(Node *node)
{
    if (node->kind != ND_IF)
        error("if文ではありません");
    bool else_exist = node->on_else;
    int lab = node->id;
    gen(node->condition);
    pop(ILRG_RAX);
    new_il_sentence_raw("  cmp rax, 0");
    if (else_exist)
        new_il_sentence_raw("  je .Lelse%d", lab);
    else
        new_il_sentence_raw("  je .Lend%d", lab);
    gen(node->body);
    if (else_exist)
    {
        new_il_sentence_raw("  jmp .Lend%d", lab);
        new_il_sentence_raw(".Lelse%d:", lab);
        gen(node->on_else);
    }
    new_il_sentence_raw(".Lend%d:", lab);
}

static void gen_function(Node *node) // gen_function_callとかのほうがいい気がする
{
    if (node->kind != ND_FUNC)
    {
        error("関数ではありません");
    }
    if (strncmp(node->name, "__builtin", h_strlen("__builtin")) == 0)
    {
        gen_builtin_function(node);
        return;
    }
    Node *arg = node->next;
    Node *first_arg = NULL;
    bool isfloat[6];
    int count = 0;
    int num_of_float_arg = 0;
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
        if (is_float(arg->ty))
        {
            isfloat[count] = true;
            num_of_float_arg++;
        }
        else
        {
            isfloat[count] = false;
        }
        arg = arg->next;
        count++;
    }
    while (count > 0)
    {
        count--;
        if (isfloat[count])
        {
            num_of_float_arg--;
            new_il_sentence_raw("  movss %s, [rsp]", xmm[num_of_float_arg]);
            new_il_sentence_raw("  add rsp, 8");
            ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
        }
        else
        {
            pop(count - num_of_float_arg);
        }
    }
    if (!ctx->is_aligned_stack_ptr)
    {
        new_il_sentence_raw("  sub rsp, 8");
    }
    new_il_sentence_raw("  mov al, 0");
    if (node->is_static)
    {
        new_il_sentence_raw("  call L%.*s", node->length, node->name);
    }
    else
    {
        new_il_sentence_raw("  call %.*s", node->length, node->name);
    }

    if (!ctx->is_aligned_stack_ptr)
    {
        new_il_sentence_raw("  add rsp, 8");
    }
    push(ILRG_RAX);
}

static void gen_global_var_def(Node *node)
{
    if (node->kind != ND_GVARDEF)
    {
        error("グローバル変数定義ではありません");
    }
    if (node->gvar_init)
    {
        Node *cur = node->gvar_init;
        while (cur)
        {
            if (cur->kind == ND_STRING)
            {
                new_il_sentence_raw_to_data(".LC%d:", cur->id);
                new_il_sentence_raw_to_data("  .string \"%.*s\"", cur->str_len, cur->str_content);
            }
            else if (cur->child && cur->child->kind == ND_STRING)
            {
                new_il_sentence_raw_to_data(".LC%d:", cur->child->id);
                new_il_sentence_raw_to_data("  .string \"%.*s\"", cur->child->str_len, cur->child->str_content);
            }
            cur = cur->next;
        }
    }
    if (node->is_static)
    {
        new_il_sentence_raw_to_data("L%.*s.%d:", node->length, node->name, node->variable_id);
    }
    else
    {
        new_il_sentence_raw_to_data(".globl %.*s", node->length, node->name);
        new_il_sentence_raw_to_data("%.*s:", node->length, node->name);
    }
    if (node->gvar_init)
    {

        if (is_int_or_char(node->ty))
        {
            new_il_sentence_raw_to_data("  .long %d", node->gvar_init->val);
        }
        else if (is_char(node->ty->ptr_to))
        {
            new_il_sentence_raw_to_data("  .quad .LC%d", node->gvar_init->id);
        }
        else
        {
            Node *cur = node->gvar_init;
            int counter = 0;
            while (cur)
            {
                if (cur->child->kind == ND_STRING)
                {
                    new_il_sentence_raw_to_data("  .quad .LC%d", cur->child->id);
                }
                else
                {
                    new_il_sentence_raw_to_data("  .long %d", cur->child->val);
                }
                cur = cur->next;
                counter++;
            }
            int remain_size = (node->ty->array_size - counter) * calc_bytes(node->ty->ptr_to);
            if (remain_size)
            {
                new_il_sentence_raw_to_data("  .zero %d", remain_size);
            }
            return;
        }
    }
    else
    {
        new_il_sentence_raw_to_data("  .zero  %d", calc_bytes(node->ty));
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
            new_il_sentence_raw("  lea rax, L%.*s.%d[rip]", node->length, node->name, node->variable_id);
            push(ILRG_RAX);
        }
        else if (node->is_local)
        {
            new_il_sentence_raw("  mov rax, rbp");
            new_il_sentence_raw("  sub rax, %d", node->offset);
            push(ILRG_RAX);
        }
        else if (node->is_static)
        {
            new_il_sentence_raw("  lea rax, L%.*s.%d[rip]", node->length, node->name, node->variable_id);
            push(ILRG_RAX);
        }
        else
        {
            new_il_sentence_raw("  lea rax, %.*s[rip]", node->length, node->name);
            push(ILRG_RAX);
        }
        return;
    case ND_MEMBER:
        gen_addr(node->child);
        pop(ILRG_RAX);
        new_il_sentence_raw("  add rax, %d", node->member->offset);
        push(ILRG_RAX);
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
        new_il_sentence_raw("L%.*s:", node->length, node->name);
    }
    else
    {
        new_il_sentence_raw(".globl %.*s", node->length, node->name);
        new_il_sentence_raw("%.*s:", node->length, node->name);
    }
    // プロローグ
    push(ILRG_RBP);
    new_il_sentence_raw("  mov rbp, rsp");
    int variable_region_size = 16 * (node->args_region_size / 16 + 1);
    if (node->has_variable_length_arguments)
    {
        variable_region_size += 80;
    }
    new_il_sentence_raw("  sub rsp, %d", variable_region_size);

    if (node->has_variable_length_arguments)
    {
        // レジスタの中身を所定の位置に書き出す
        new_il_sentence_raw("  mov QWORD PTR -48[rbp], rdi");
        new_il_sentence_raw("  mov QWORD PTR -40[rbp], rsi");
        new_il_sentence_raw("  mov QWORD PTR -32[rbp], rdx");
        new_il_sentence_raw("  mov QWORD PTR -24[rbp], rcx");
        new_il_sentence_raw("  mov QWORD PTR -16[rbp], r8");
        new_il_sentence_raw("  mov QWORD PTR -8[rbp], r9");
    }

    // 第1〜6引数をローカル変数の領域に書き出す
    int count = 0;
    int count_float = 0;
    Node *arg = node->lhs;
    while (arg != NULL)
    {
        gen_addr(arg);
        pop(ILRG_RAX);
        if (count < 6)
        {
            char *reg;
            if (is_float(arg->ty))
            {
                reg = xmm[count_float];
                new_il_sentence_raw("  movss [rax], %s", reg);
                count_float++;
            }
            else
            {
                reg = get_register_from_type(count - count_float, arg->ty);
                new_il_sentence_raw("  mov [rax], %s", reg);
            }
        }
        else
        {
            error("引数の数が多すぎます");
        }
        count++;
        arg = arg->lhs;
    }

    new_il_sentence_raw("  and rsp, 0xfffffffffffffff0");
    ctx->is_aligned_stack_ptr = true;

    gen(node->rhs);
    pop(ILRG_RAX);

    // エピローグ
    // ここに書くと多分returnなしで戻り値を指定できるようになってしまう、どうすべきか
    new_il_sentence_raw("  mov rsp, rbp");
    pop(ILRG_RBP);
    new_il_sentence_raw("  ret");
}

static void gen_assign(Node *node)
{
    if (node->kind != ND_ASSIGN)
    {
        error("代入式ではありません");
    }

    // 初期化式
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
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            new_il_sentence_raw("  add rax, %d", calc_bytes(node->lhs->ty->ptr_to) * counter);
            if (is_int(node->lhs->ty->ptr_to))
                new_il_sentence_raw("  mov [rax], edi");
            else if (is_char(node->lhs->ty->ptr_to))
            {
                new_il_sentence_raw("  mov [rax], dil");
            }
            else
                new_il_sentence_raw("  mov [rax], rdi");
            cur = cur->next;
            counter++;
        }
        return;
    }

    gen_addr(node->lhs);
    gen(node->rhs);
    pop(ILRG_RDI);
    pop(ILRG_RAX);

    if (is_int(node->ty))
    {
        new_il_sentence_raw("  mov [rax], edi");
    }
    else if (is_float(node->ty))
    {
        new_il_sentence_raw("  movss DWORD PTR [rax], xmm0");
    }
    else if (is_char(node->ty))
    {
        new_il_sentence_raw("  mov [rax], dil");
    }
    else
    {
        new_il_sentence_raw("  mov [rax], rdi");
    }
    push(ILRG_RDI);
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

static void logging(Node *node)
{
    if (!opts->is_verbose)
        return;
    int lhs = -1;
    int rhs = -1;
    int body = -1;
    if (node->lhs != NULL)
    {
        lhs = node->lhs->kind;
    }
    if (node->rhs != NULL)
    {
        rhs = node->rhs->kind;
    }
    if (node->body != NULL)
    {
        body = node->body->kind;
    }
    printf("Kind: %d, Left: %d, Right: %d, Body: %d\n", node->kind, lhs, rhs, body);
}

static void gen(Node *node)
{
    logging(node);
    switch (node->kind)
    {
    case ND_NUM:
        push_val(node->val);
        return;
    case ND_FLOAT:
        gen_float(node);
        return;
    case ND_NOT:
        gen(node->child);
        pop(ILRG_RAX);
        new_il_sentence_raw("  cmp rax, 0");
        new_il_sentence_raw("  sete al");
        new_il_sentence_raw("  movzb rax, al");
        push(ILRG_RAX);
        return;
    case ND_VAR:
        gen_addr(node);
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        pop(ILRG_RAX);
        if (is_int(node->ty))
            new_il_sentence_raw("  mov eax, [rax]");
        else if (is_float(node->ty))
        {
            new_il_sentence_raw("  movss xmm0, DWORD PTR [rax]");
            new_il_sentence_raw("  mov eax, [rax]");
        }
        else if (is_char(node->ty))
        {
            new_il_sentence_raw("  movsx eax, BYTE PTR [rax]");
        }
        else
            new_il_sentence_raw("  mov rax, [rax]");
        push(ILRG_RAX);
        return;
    case ND_ASSIGN:
        gen_assign(node);
        return;
    case ND_RETURN:
        gen(node->child);
        pop(ILRG_RAX);
        new_il_sentence_raw("  mov rsp, rbp");
        pop(ILRG_RBP);
        new_il_sentence_raw("  ret");
        return;
    case ND_IF:
        gen_if(node);
        return;
    case ND_FOR:
    case ND_WHILE:
        gen_loop(node);
        return;
    case ND_BLOCK:
        gen_block(node);
        return;
    case ND_SWITCH:
        gen(node->condition);
        pop(ILRG_RAX);
        for (Node *c = node->next_case; c; c = c->next_case)
        {
            new_il_sentence_raw("  cmp eax, %d", c->val);
            new_il_sentence_raw("  je .L.Case%d", c->id);
        }
        if (node->default_case)
        {
            new_il_sentence_raw("  jmp .L.Case%d", node->default_case->id);
        }
        new_il_sentence_raw("  jmp .L.End%d", node->id);
        gen(node->child);
        new_il_sentence_raw(".L.End%d:", node->id);
        return;
    case ND_CASE:
        new_il_sentence_raw(".L.Case%d:", node->id);
        gen(node->child);
        return;
    case ND_BREAK:
        new_il_sentence_raw("  jmp .L.End%d", node->break_to_id);
        return;
    case ND_CONTINUE:
        new_il_sentence_raw("  jmp .L.OnEnd%d", node->continue_to_id);
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
        push_str_addr(node->id);
        new_il_sentence_raw_to_data(".LC%d:", node->id);
        new_il_sentence_raw_to_data("  .string \"%.*s\"", node->str_len, node->str_content);
        return;
    case ND_MEMBER:
        gen_addr(node);
        if (node->ty->ty == ARRAY)
        {
            return;
        }
        pop(ILRG_RAX);
        if (is_float(node->member->ty))
        {
            new_il_sentence_raw("  movss xmm0, DWORD PTR [rax]");
            new_il_sentence_raw("  mov eax, DWORD PTR [rax]");
        }
        else if (is_int(node->member->ty))
            new_il_sentence_raw("  mov eax, [rax]");
        else if (is_char(node->member->ty))
        {
            new_il_sentence_raw("  movsx eax, BYTE PTR [rax]");
        }
        else
            new_il_sentence_raw("  mov rax, [rax]");
        push(ILRG_RAX);
        return;
    case ND_ADDR:
        gen_addr(node->child);
        return;
    case ND_DEREF:
        gen(node->child);
        pop(ILRG_RAX);
        if (is_int(node->ty))
            new_il_sentence_raw("  mov eax, [rax]");
        else if (is_char(node->ty))
        {
            new_il_sentence_raw("  movsx eax, BYTE PTR [rax]");
        }
        else
            new_il_sentence_raw("  mov rax, [rax]");
        push(ILRG_RAX);
        return;
    case ND_POSTINC:
    case ND_POSTDEC:
        gen(node->lhs); // インクリメント前の値がpushされる
        gen(node->rhs); // インクリメント後の値がpushされる
        pop(ILRG_RAX);  // インクリメント後の値がpopされる
        // スタックトップはインクリメント前の値
        return;
    case ND_ADD:
    case ND_SUB:
        gen(node->lhs);
        gen(node->rhs);
        if (is_int_or_char(node->ty))
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            if (node->kind == ND_ADD)
                new_il_sentence_raw("  add eax, edi");
            else
                new_il_sentence_raw("  sub eax, edi");
        }
        else if (is_float(node->ty))
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            if (node->kind == ND_ADD)
                new_il_sentence_raw("  addss xmm0, xmm1");
            else
                new_il_sentence_raw("  subss xmm0, xmm1");
            new_il_sentence_raw("  movss 8[rsp], xmm0");
            new_il_sentence_raw("  add rsp, 8");
            ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
            ;
            return;
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            int size = calc_bytes(node->ty->ptr_to);
            if (is_int_or_char(node->lhs->ty))
            {
                new_il_sentence_raw("  imul rax, %d", size);
            }
            else if (is_int_or_char(node->rhs->ty))
            {
                new_il_sentence_raw("  imul rdi, %d", size);
            }
            else if (is_ptr(node->rhs->ty) && is_ptr(node->lhs->ty) && node->kind == ND_SUB)
            {
                new_il_sentence_raw("  sub rax, rdi");
                new_il_sentence_raw("  cqo");
                new_il_sentence_raw("  mov rdi, %d", size);
                new_il_sentence_raw("  idiv rdi");
                push(ILRG_RAX);
                return;
            }
            else
            {
                error("適切でない演算です");
            }

            if (node->kind == ND_ADD)
                new_il_sentence_raw("  add rax, rdi");
            else
                new_il_sentence_raw("  sub rax, rdi");
        }
        push(ILRG_RAX);
        return;
    case ND_AND:
    case ND_OR:
        gen_logical_operator(node);
    case ND_TYPE:
    case ND_NOP:
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    switch (node->kind)
    {
    case ND_MUL:
        if (is_float(node->ty))
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            new_il_sentence_raw("  mulss xmm0, xmm1");
            new_il_sentence_raw("  movss 8[rsp], xmm0");
            new_il_sentence_raw("  add rsp, 8");
            ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
            ;
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);

            new_il_sentence_raw("  imul eax, edi");
            push(ILRG_RAX);
        }
        break;
    case ND_DIV:
        if (is_float(node->ty))
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            new_il_sentence_raw("  divss xmm0, xmm1");
            new_il_sentence_raw("  movss 8[rsp], xmm0");
            new_il_sentence_raw("  add rsp, 8");
            ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            new_il_sentence_raw("  cdq");
            new_il_sentence_raw("  idiv edi");
            push(ILRG_RAX);
        }
        break;
    case ND_MOD:
        pop(ILRG_RDI);
        pop(ILRG_RAX);
        new_il_sentence_raw("  cdq");
        new_il_sentence_raw("  idiv edi");
        push(ILRG_RDX);
        break;
    case ND_EQUAL:
        if (node->lhs->ty->ty == FLOAT)
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            new_il_sentence_raw("  add rsp, 16");
            new_il_sentence_raw("  ucomiss xmm0, xmm1");
            new_il_sentence_raw("  sete al");
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            new_il_sentence_raw("  cmp rax, rdi");
            new_il_sentence_raw("  sete al");
        }
        new_il_sentence_raw("  movzb rax, al");
        push(ILRG_RAX);
        break;
    case ND_GEQ:
        if (node->lhs->ty->ty == FLOAT)
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            new_il_sentence_raw("  add rsp, 16");
            new_il_sentence_raw("  comiss xmm0, xmm1");
            new_il_sentence_raw("  setnb al");
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            new_il_sentence_raw("  movsxd rdi, edi");
            new_il_sentence_raw("  movsxd rax, eax");
            new_il_sentence_raw("  cmp rax, rdi");
            new_il_sentence_raw("  setge al");
        }
        new_il_sentence_raw("  movzb rax, al");
        push(ILRG_RAX);
        break;
    case ND_GTH:
        if (node->lhs->ty->ty == FLOAT)
        {
            new_il_sentence_raw("  movss xmm0, 8[rsp]");
            new_il_sentence_raw("  movss xmm1, [rsp]");
            new_il_sentence_raw("  add rsp, 16");
            new_il_sentence_raw("  comiss xmm0, xmm1");
            new_il_sentence_raw("  seta al");
        }
        else
        {
            pop(ILRG_RDI);
            pop(ILRG_RAX);
            new_il_sentence_raw("  movsxd rdi, edi");
            new_il_sentence_raw("  movsxd rax, eax");
            new_il_sentence_raw("  cmp rax, rdi");
            new_il_sentence_raw("  setg al");
        }
        new_il_sentence_raw("  movzb rax, al");
        push(ILRG_RAX);
        break;
    }
}

ILProgram *generate_inter_language(Node **nodes)
{
    ILProgram *program = calloc(1, sizeof(ILProgram));
    ILSentence *head = calloc(1, sizeof(ILSentence));
    ILSentence *data_head = calloc(1, sizeof(ILSentence));
    cur_text = head;
    cur_data = data_head;
    if (opts->is_verbose)
    {
        printf("\x1b[33mSTART GENERATING\x1b[0m\n");
    }
    int i = 0;
    while (nodes[i] != NULL)
    {
        gen(nodes[i]);
        i++;
    }
    if (opts->is_verbose)
    {
        printf("\x1b[33mEND GENERATING\x1b[0m\n");
    }

    program->data = data_head->next;
    program->text = head->next;
    return program;
}

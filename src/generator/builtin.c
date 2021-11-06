#include "../hooligan.h"

static void gen_va_start(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_start", h_strlen("__builtin_va_start")) != 0)
    {
        error("va_startではありません");
    }
    Node *first_arg = node->next_arg->child;
    new_il_sentence_raw("  mov DWORD PTR -72[rbp], 8");  // TODO 本当は第二引数から計算しないといけない
    new_il_sentence_raw("  mov DWORD PTR -68[rbp], 48"); // va_list->fp_offset
    new_il_sentence_raw("  lea rax, 16[rbp]");
    new_il_sentence_raw("  mov QWORD PTR -64[rbp], rax"); // va_list->overflow_arg_area
    new_il_sentence_raw("  lea rax, -48[rbp]");
    new_il_sentence_raw("  mov QWORD PTR -56[rbp], rax"); // va_list->reg_arg_area
    new_il_sentence_raw("  lea rdi, -72[rbp]");
    new_il_sentence_raw("  mov rax, rbp");
    new_il_sentence_raw("  sub rax, %d", first_arg->offset);
    new_il_sentence_raw("  mov [rax], rdi");
}

static void gen_va_arg(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_arg", h_strlen("__builtin_va_arg")) != 0)
    {
        error("va_argではありません");
    }
    Node *first_arg = node->next_arg->child;
    Node *second_arg = node->next_arg->next_arg->child;
    if (second_arg->kind != ND_TYPE)
    {
        error("va_argの第2引数には型を指定してください");
    }

    new_il_sentence_raw("  mov rax, rbp");
    new_il_sentence_raw("  sub rax, %d", first_arg->offset);
    new_il_sentence_raw("  mov rdi, 0");
    new_il_sentence_raw("  mov rax, [rax]");
    new_il_sentence_raw("  mov edi, DWORD PTR [rax]");
    new_il_sentence_raw("  add rax, 16");
    new_il_sentence_raw("  mov rax, [rax]");
    new_il_sentence_raw("  add rax, rdi");
    new_il_sentence_raw("  add edi, 8");
    new_il_sentence_raw("  mov DWORD PTR -72[rbp], edi");
    if (is_int(second_arg->ty))
        new_il_sentence_raw("  mov eax, [rax]");
    else if (is_char(second_arg->ty))
    {
        new_il_sentence_raw("  movsx eax, BYTE PTR [rax]");
    }
    else
        new_il_sentence_raw("  mov rax, [rax]");
    push(ILRG_RAX);
}

static void gen_va_end(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_end", h_strlen("__builtin_va_end")) != 0)
    {
        error("va_endではありません");
    }
    // TODO va_end実装
}

void gen_builtin_function(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin", h_strlen("__builtin")) != 0)
    {
        error("ビルトイン関数ではありません");
    }

    if (strncmp(node->name, "__builtin_va_start", h_strlen("__builtin_va_start")) == 0)
    {
        gen_va_start(node);
    }
    else if (strncmp(node->name, "__builtin_va_arg", h_strlen("__builtin_va_arg")) == 0)
    {
        gen_va_arg(node);
    }
    else if (strncmp(node->name, "__builtin_va_end", h_strlen("__builtin_va_end")) == 0)
    {
        gen_va_end(node);
    }
    else
    {
        error("存在しないビルトイン関数です");
    }
}

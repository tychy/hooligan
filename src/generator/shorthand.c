#include "../hooligan.h"

static void push(ILRegister r)
{
    new_il_sentence_single_operand(ILST_PUSH, new_il_operand_reg(r));
}

static void push_val(int val)
{
    new_il_sentence_raw("  push %d", val);
    ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
}

static void push_str_addr(int label)
{
    new_il_sentence_raw("  lea rax, QWORD PTR .LC%d[rip]", label);
    push(ILRG_RAX);
}

static void pop(ILRegister r)
{
    new_il_sentence_single_operand(ILST_POP, new_il_operand_reg(r));
}

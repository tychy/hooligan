#include "hooligan.h"

ILOperand *new_il_operand_reg(ILRegister reg)
{
    ILOperand *op = calloc(1, sizeof(ILOT_REG));
    op->ty = ILOT_REG;
    op->reg = reg;
    return op;
}

char *il_operand_to_str(ILOperand *op)
{
    if (op->ty == ILOT_REG)
    {
        switch (op->reg)
        {
        case ILRG_RDI:
            return "rdi";
        case ILRG_RSI:
            return "rsi";
        case ILRG_RDX:
            return "rdx";
        case ILRG_RCX:
            return "rcx";
        case ILRG_R8:
            return "r8";
        case ILRG_R9:
            return "r9";
        case ILRG_RAX:
            return "rax";
        case ILRG_RBP:
            return "rbp";
        case ILRG_RSP:
            return "rsp";
        default:
            error("not implemented");
        }
    }
    else
    {
        error("not implemented");
    }
}

void generate_intel_syntax_assembly(ILProgram *program)
{
    if (opts->platform == PF_MACOS)
    {
        fprintf(output, "section .data\n");
    }
    else
    {
        fprintf(output, ".intel_syntax noprefix\n");
        fprintf(output, ".data\n");
    }
    ILSentence *st = program->data;
    while (st)
    {
        if (st->ty == ILST_RAW)
        {
            fprintf(output, "%s\n", st->raw_sentence);
        }
        else if (st->ty == ILST_PUSH)
        {
            fprintf(output, "  push %s\n", il_operand_to_str(st->first_operand));
        }
        else if (st->ty == ILST_POP)
        {
            fprintf(output, "  pop %s\n", il_operand_to_str(st->first_operand));
        }
        else
        {
            error("not implemented");
        }

        st = st->next;
    }

    if (opts->platform == PF_MACOS)
    {
        fprintf(output, "section .text\n");
    }
    else
    {
        fprintf(output, ".intel_syntax noprefix\n");
        fprintf(output, ".text\n");
    }
    st = program->text;
    while (st)
    {
        if (st->ty == ILST_RAW)
        {
            fprintf(output, "%s\n", st->raw_sentence);
        }
        else if (st->ty == ILST_PUSH)
        {
            fprintf(output, "  push %s\n", il_operand_to_str(st->first_operand));
        }
        else if (st->ty == ILST_POP)
        {
            fprintf(output, "  pop %s\n", il_operand_to_str(st->first_operand));
        }
        else
        {
            error("not implemented");
        }

        st = st->next;
    }
}

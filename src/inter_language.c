#include "inter_language.h"
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

ILSentence *new_il_sentence_raw(char *raw_sentence)
{
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ILST_RAW;
    st->raw_sentence = raw_sentence;
    return st;
}

ILSentence *new_il_sentence_push(ILRegister reg)
{
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ILST_PUSH;
    st->first_operand = new_il_operand_reg(reg);
    return st;
}

ILSentence *new_il_sentence_single_operand(ILSentenceType ty, ILOperand *op)
{
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ty;
    st->first_operand = op;
    return st;
}

void generate_intel_syntax_assembly(ILSentence *ils)
{
    while (ils)
    {
        if (ils->ty == ILST_RAW)
        {
            fprintf(output, "%s\n", ils->raw_sentence);
            // fprintf(output, "\n");
        }
        else if (ils->ty == ILST_PUSH)
        {
            fprintf(output, "  push %s\n", il_operand_to_str(ils->first_operand));
        }
        else if (ils->ty == ILST_POP)
        {
            fprintf(output, "  pop %s\n", il_operand_to_str(ils->first_operand));
        }
        else
        {
            error("not implemented");
        }

        ils = ils->next;
    }
}

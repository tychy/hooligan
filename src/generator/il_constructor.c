#include "../hooligan.h"

static ILSentence *cur;
static ILSentence *cur_data;

static ILSentence *new_il_sentence_raw(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *raw = calloc(100, sizeof(char));
    vsprintf(raw, fmt, ap);
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ILST_RAW;
    st->raw_sentence = raw;
    cur->next = st;
    cur = cur->next;
    va_end(ap);
    return st;
}

static ILSentence *new_il_sentence_raw_to_data(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char *raw = calloc(100, sizeof(char));
    vsprintf(raw, fmt, ap);
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ILST_RAW;
    st->raw_sentence = raw;
    cur_data->next = st;
    cur_data = cur_data->next;
    va_end(ap);
    return st;
}

ILSentence *new_il_sentence_single_operand(ILSentenceType ty, ILOperand *op)
{
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ty;
    st->first_operand = op;
    cur->next = st;
    cur = cur->next;
    if (ty == ILST_PUSH || ty == ILST_POP)
    {
        ctx->is_aligned_stack_ptr = !ctx->is_aligned_stack_ptr;
    }
    return st;
}

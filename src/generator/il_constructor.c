#include "../hooligan.h"

static ILSentence *cur_text;
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
    cur_text->next = st;
    cur_text = cur_text->next;
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
    cur_text->next = st;
    cur_text = cur_text->next;
    return st;
}

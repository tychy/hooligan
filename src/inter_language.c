#include "inter_language.h"
#include "hooligan.h"

ILSentence *new_il_sentence_raw(char *raw_sentence)
{
    ILSentence *st = calloc(1, sizeof(ILSentence));
    st->ty = ILST_RAW;
    st->raw_sentence = raw_sentence;
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
        else
        {
            error("not implemented");
        }

        ils = ils->next;
    }
}

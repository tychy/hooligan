#include "hooligan.h"

ILProgram *optimize_inter_language(ILProgram *program)
{
    ILSentence *cur = program->text;
    ILSentence *next;
    while (cur)
    {
        next = cur->next;
        if (next && next->ty == ILST_PUSH)
        {
            ILSentence *push = next;
            if (next->next && next->next->ty == ILST_POP)
            {
                ILSentence *pop = next->next;
                bool is_same_reg = push->first_operand->ty == pop->first_operand->ty && push->first_operand->reg == pop->first_operand->reg;

                if (is_same_reg && pop->next)
                {
                    cur->next = pop->next;
                    continue;
                }
            }
        }

        cur = cur->next;
    }
    return program;
}

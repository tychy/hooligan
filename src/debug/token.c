#include "../hooligan.h"

static void debug_tokens(Token *tok)
{
    Token *cur = tok;
    while (cur)
    {
        switch (cur->kind)
        {
        case TK_CHARACTER:
            printf("%d ", cur->val);
            break;
        case TK_HEADER_NAME:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_PPDIRECTIVE:
        case TK_RESERVED_WORD:
        case TK_IDENT:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_NUMBER:
            if (cur->is_float)
            {
                printf("%f ", cur->float_val);
                break;
            }
            else
            {
                printf("%d ", cur->val);
                break;
            }
        case TK_OPERATOR:
            printf("%.*s ", cur->len, cur->str);
            break;
        case TK_STRING:
            printf("\\\"%.*s\\\" ", cur->len, cur->str);
            break;
        }
        cur = cur->next;
    }
    printf("\n");
}

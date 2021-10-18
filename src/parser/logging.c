#include "../hooligan.h"

static void logging(Node *node)
{
    if (opts->is_verbose)
    {
        printf("Kind: %d\n", node->kind);
    }
}

#include "hooligan.h"

void gen_va_start(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_start", strlen("__builtin_va_start")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_startの実装
}

void gen_va_arg(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_arg", strlen("__builtin_va_arg")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_argの実装
}

void gen_va_end(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_end", strlen("__builtin_va_end")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_end実装
}

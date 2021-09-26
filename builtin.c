#include "hooligan.h"

static void gen_va_start(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_start", strlen("__builtin_va_start")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_startの実装
    Node *first = node->next;
    // printf("%.*s\n", first->child->length, first->child->name);
    // スタックへのポインタがほしい
    Node *second = first->next;
    // printf("%.*s\n", second->child->length, second->child->name);
}

static void gen_va_arg(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_arg", strlen("__builtin_va_arg")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_argの実装
}

static void gen_va_end(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin_va_end", strlen("__builtin_va_end")) != 0)
    {
        error("va_startではありません");
    }
    // TODO va_end実装
}

void gen_builtin_function(Node *node)
{
    if (node->kind != ND_FUNC || strncmp(node->name, "__builtin", strlen("__builtin")) != 0)
    {
        error("ビルトイン関数ではありません");
    }

    if (strncmp(node->name, "__builtin_va_start", strlen("__builtin_va_start")) == 0)
    {
        gen_va_start(node);
    }
    else if (strncmp(node->name, "__builtin_va_arg", strlen("__builtin_va_arg")) == 0)
    {
        gen_va_arg(node);
    }
    else if (strncmp(node->name, "__builtin_va_end", strlen("__builtin_va_end")) == 0)
    {
        gen_va_end(node);
    }
    else
    {
        error("存在しないビルトイン関数です");
    }
}

#include "../hooligan.h"

static void new_scope()
{
    Scope *scope = calloc(1, sizeof(Scope));
    scope->prev = ctx->scope;
    ctx->scope_serial_num++;
    scope->label = ctx->scope_serial_num;
    scope->loop_label = ctx->scope->loop_label;
    ctx->scope->next = scope;
    ctx->scope = scope;
}

static void exit_scope()
{
    ctx->scope = ctx->scope->prev;
}

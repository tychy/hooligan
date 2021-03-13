#include "hooligan.h"

void new_scope()
{
    Scope *scope = calloc(1, sizeof(Node));
    scope->prev = ctx->scope;
    ctx->scope_serial_num++;
    scope->label = ctx->scope_serial_num;
    scope->loop_label = ctx->scope->loop_label;
    ctx->scope->next = scope;
    ctx->scope = scope;
}

void exit_scope()
{
    ctx->scope = ctx->scope->prev;
}

void start_loop()
{
    new_scope();
    ctx->scope->loop_label = ctx->scope->label;
}

void end_loop()
{
    exit_scope();
}

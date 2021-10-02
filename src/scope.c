#include "hooligan.h"

void new_scope()
{
    Scope *scope = calloc(1, sizeof(Scope));
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
    ctx->break_to = ctx->scope->loop_label;
    ctx->continue_to = ctx->scope->loop_label;
}

void end_loop()
{
    exit_scope();
    ctx->break_to = ctx->scope->loop_label;
    ctx->continue_to = ctx->scope->loop_label;
}

void start_switch()
{
    new_scope();
    ctx->scope->loop_label = ctx->scope->label;
    ctx->break_to = ctx->scope->loop_label;
}

void end_switch()
{
    exit_scope();
    ctx->break_to = ctx->scope->loop_label;
}

int get_unique_num()
{
    return ctx->scope_serial_num++;
}

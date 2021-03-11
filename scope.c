#include "hooligan.h"

Scope *current_scope;

void new_scope()
{
    Scope *scope = calloc(1, sizeof(Node));
    scope->prev = current_scope;
    current_scope->next = scope;
    current_scope = scope;
}

void exit_scope()
{
    current_scope = current_scope->prev;
}

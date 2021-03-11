#include "hooligan.h"

Scope *current_scope;
int scope_label = 1;

void new_scope()
{
    Scope *scope = calloc(1, sizeof(Node));
    scope->prev = current_scope;
    scope_label++;
    scope->label = scope_label;
    current_scope->next = scope;
    current_scope = scope;
}

void exit_scope()
{
    current_scope = current_scope->prev;
}

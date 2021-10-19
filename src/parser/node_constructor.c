#include "../hooligan.h"

static Node *new_node_raw(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    logging(node);
    return node;
}

static String *new_string(char *p, int length)
{
    String *new_string = calloc(1, sizeof(String));
    new_string->length = length;
    new_string->p = p;
    new_string->label = ctx->data_label++;
    new_string->next = ctx->strings;
    ctx->strings = new_string;
    return new_string;
}

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node_raw(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = determine_expr_type(lhs->ty, rhs->ty);
    if (!node->ty)
    {
        error("式にはintが必要です");
    }
    return node;
}

static Node *new_node_inc(Node *lhs, Node *rhs)
{
    if (lhs->ty->ty != rhs->ty->ty || lhs->ty->ptr_to != rhs->ty->ptr_to)
    {
        error("lhsとrhsの型は同一である必要があります\n");
    }

    Node *node = new_node_raw(ND_POSTINC);
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = lhs->ty;
    return node;
}

static Node *new_node_dec(Node *lhs, Node *rhs)
{
    if (lhs->ty->ty != rhs->ty->ty || lhs->ty->ptr_to != rhs->ty->ptr_to)
    {
        error("lhsとrhsの型は同一である必要があります\n");
    }
    Node *node = new_node_raw(ND_POSTDEC);
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = lhs->ty;
    return node;
}

static Node *new_node_single(NodeKind kind, Node *child)
{
    Node *node = new_node_raw(kind);
    node->child = child;
    if (kind == ND_DEREF)
    {
        if (is_int_or_char(child->ty))
            error("pointer型である必要があります");
        node->ty = child->ty->ptr_to;
    }
    else if (kind == ND_ADDR)
        node->ty = new_type_ptr(node->child->ty);
    else
        node->ty = child->ty;
    return node;
}

static Node *new_node_assign(Node *lhs, Node *rhs)
{
    Node *node = new_node_raw(ND_ASSIGN);
    node->lhs = lhs;
    node->rhs = rhs;
    if (node->rhs->ty->ty == VOID)
    {
        error("void型の値は無視しなくてはいけません");
    }
    node->ty = lhs->ty;
    return node;
}

static Node *new_node_num(int val)
{
    Node *node = new_node_raw(ND_NUM);
    node->val = val;
    node->ty = new_type_int();
    return node;
}

static Node *new_node_float(int integer, int decimal, int numzero)
{
    Float *new_float = calloc(1, sizeof(Float));
    new_float->integer = integer;
    new_float->decimal = decimal;
    new_float->numzero = numzero;
    new_float->label = ctx->data_label++;
    new_float->next = ctx->floats;
    ctx->floats = new_float;

    Node *node = new_node_raw(ND_FLOAT);
    node->data_label = new_float->label;
    node->ty = new_type_float();
    return node;
}

static Node *new_node_nop()
{
    Node *node = new_node_raw(ND_NOP);
    return node;
}

static Node *new_node_var(Var *var)
{
    Node *node = new_node_raw(ND_VAR);
    if (var->is_local)
    {
        node->offset = var->offset;
    }
    node->name = var->name;
    node->length = var->length;
    node->ty = var->ty;
    node->is_local = var->is_local;
    node->is_static = var->is_static;
    if (node->is_static)
    {
        node->scope_label = var->label;
    }
    return node;
}

static Node *new_node_string(String *s)
{
    Node *node = new_node_raw(ND_STRING);
    node->strlabel = s->label;
    node->ty = new_type_string();
    return node;
}
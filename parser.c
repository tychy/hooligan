#include "hooligan.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_var(int offset)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    node->offset = offset;
    return node;
}

Node *num()
{
    return new_node_num(expect_number());
}

Node *ident()
{
    return new_node_var(expect_var());
}

Node *primary()
{
    if (consume("("))
    {
        Node *node = expr();
        expect(")");
        return node;
    }
    else if (token->kind == TK_IDENT)
    {
        return ident();
    }
    else
    {
        return num();
    }
}

Node *unary()
{
    if (consume("+"))
    {
        return primary();
    }
    else if (consume("-"))
    {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *mul()
{
    Node *node = unary();
    for (;;)
    {
        if (consume("*"))
        {
            node = new_node(ND_MUL, node, unary());
        }
        else if (consume("/"))
        {
            node = new_node(ND_DIV, node, unary());
        }
        else
        {
            return node;
        }
    }
}

Node *add()
{
    Node *node = mul();
    for (;;)
    {
        if (consume("+"))
        {
            node = new_node(ND_ADD, node, mul());
        }
        else if (consume("-"))
        {
            node = new_node(ND_SUB, node, mul());
        }
        else
        {
            return node;
        }
    }
}

Node *relational()
{
    Node *node = add();
    for (;;)
    {

        if (consume(">="))
        {
            node = new_node(ND_GEQ, node, add());
        }
        else if (consume("<="))
        {
            node = new_node(ND_LEQ, node, add());
        }
        else if (consume(">"))
        {
            node = new_node(ND_GTH, node, add());
        }
        else if (consume("<"))
        {
            node = new_node(ND_LTH, node, add());
        }
        else
        {
            return node;
        }
    }
}

Node *equality()
{
    Node *node = relational();
    for (;;)
    {

        if (consume("=="))
        {
            node = new_node(ND_EQUAL, node, relational());
        }
        else if (consume("!="))
        {
            node = new_node(ND_NEQUAL, node, relational());
        }
        else
        {
            return node;
        }
    }
}

Node *assign()
{
    Node *node = equality();
    if (consume("="))
    {
        node = new_node(ND_ASSIGN, node, assign());
    }

    return node;
}

Node *expr()
{
    return assign();
}

Node *stmt()
{
    Node *node;
    if (consume_return())
    {
        node = new_node(ND_RETURN, expr(), NULL);
    }
    else
    {
        node = expr();
    }
    expect(";");
    return node;
}

void program()
{
    int i = 0;
    while (!at_eof())
    {
        Node *node = stmt();
        nodes[i] = node;
        i++;
    }
    nodes[i + 1] = NULL;
}

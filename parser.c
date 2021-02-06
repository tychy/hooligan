#include "hooligan.h"

Node *expr();

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

Node *num()
{
    return new_node_num(expect_number());
}

Node *primary()
{
    if (consume("("))
    {
        Node *node = expr();
        expect(")");
        return node;
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

Node *equality()
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

Node *expr()
{
    Node *node = equality();
    for (;;)
    {

        if (consume("=="))
        {
            node = new_node(ND_EQUAL, node, equality());
        }
        // else if (consume("-"))
        // {
        //     node = new_node(ND_SUB, node, equality());
        // }
        else
        {
            return node;
        }
    }
}

void gen(Node *node)
{
    if (node->kind == ND_NUM)
    {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    switch (node->kind)
    {
    case ND_ADD:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  add rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_SUB:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  sub rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_MUL:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  imul rax, rdi\n");
        printf("  push rax\n");
        break;
    case ND_DIV:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cqo\n");
        printf("  idiv rdi\n");
        printf("  push rax\n");
        break;
    case ND_EQUAL:
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
    }
}

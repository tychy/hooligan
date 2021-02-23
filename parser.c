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

Node *new_node_func(char *name, int length)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNC;
    node->name = name;
    node->length = length;
    return node;
}

Node *num()
{
    return new_node_num(expect_number());
}

Node *ident()
{
    Token *tok = consume_ident();
    if (consume("("))
    {
        Node *node = new_node_func(tok->string, tok->length);
        Node *arg_top = node;
        int count = 0;
        while (!consume(")"))
        {
            if (count > 0)
                expect(",");
            Node *arg = new_node(ND_ARG, expr(), NULL);
            arg_top->rhs = arg;
            arg_top = arg;
            count++;
        }
        return node;
    }
    else
    {
        int offset;
        LVar *lvar = find_lvar(tok);
        if (lvar)
        {
            offset = lvar->offset;
        }
        else
        {
            LVar *new_lvar = calloc(1, sizeof(LVar));
            new_lvar->length = tok->length;
            new_lvar->name = tok->string;
            if (locals)
            {
                offset = locals->offset + 8;
            }
            else
            {
                offset = 8;
            }
            new_lvar->offset = offset;
            new_lvar->next = locals;
            locals = new_lvar;
        }
        return new_node_var(offset);
    }
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
    if (consume("{"))
    {
        Node *cur = new_node(ND_BLOCK, NULL, NULL);
        node = cur;
        while (!consume("}"))
        {
            cur->lhs = stmt();
            cur->rhs = new_node(ND_BLOCK, NULL, NULL);
            cur = cur->rhs;
        }
        return node;
    }
    else if (consume_return())
    {
        node = new_node(ND_RETURN, expr(), NULL);
        expect(";");
    }
    else if (consume_if())
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *iftrue = stmt();
        if (consume_else())
        {
            node = new_node(ND_ELSE, iftrue, stmt());
            node = new_node(ND_IF, condition, node);
        }
        else
        {
            node = new_node(ND_IF, condition, iftrue);
        }
    }
    else if (consume_for())
    {
        Node *initial;
        Node *condition;
        Node *end;
        expect("(");
        if (consume(";"))
        {
            initial = NULL;
        }
        else
        {
            initial = expr();
            expect(";");
        }

        if (consume(";"))
        {
            condition = NULL;
        }
        else
        {
            condition = expr();
            expect(";");
        }

        if (consume(")"))
        {
            end = NULL;
        }
        else
        {
            end = expr();
            expect(")");
        }
        Node *body = stmt();
        Node *former = new_node(ND_FORINIT, initial, condition);
        Node *latter = new_node(ND_FORBODY, body, end);
        node = new_node(ND_FOR, former, latter);
    }
    else if (consume_while())
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        node = new_node(ND_WHILE, condition, stmt());
    }
    else
    {
        node = expr();
        expect(";");
    }
    return node;
}

Node *func()
{
    Node *node = new_node(ND_FUNCDEF, NULL, NULL);
    Token *tok = consume_ident();
    node->name = tok->string;
    node->length = tok->length;
    expect("(");
    expect(")");
    node->rhs = stmt();
    return node;
}

void program()
{
    int i = 0;
    while (!at_eof())
    {
        Node *node = func();
        nodes[i] = node;
        i++;
    }
    nodes[i + 1] = NULL;
}

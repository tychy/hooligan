#include "hooligan.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;

    // TODO ここのチェックはしなくていいようにリファクタしていく
    switch (node->kind)
    {
    case ND_RETURN:
    case ND_IF:
    case ND_ELSE:
    case ND_BLOCK:
    case ND_FUNC:
    case ND_ARG:
    case ND_FUNCDEF:
        return node;
    }
    Type *ty = calloc(1, sizeof(Type));
    if (lhs != NULL && rhs != NULL)
    {
        if (lhs->ty->ty == PTR && rhs->ty->ty == PTR)
        {
            if (kind != ND_ASSIGN)
                error("式にはintが必要です");
            // 右でも左でも一緒
            node->ty = lhs->ty;
        }
        else if (lhs->ty->ty == INT && rhs->ty->ty == PTR)
        {
            node->ty = rhs->ty;
        }
        else if (lhs->ty->ty == PTR && rhs->ty->ty == INT)
        {
            node->ty = lhs->ty;
        }
        else if (lhs->ty->ty == INT && rhs->ty->ty == INT)
        {
            // 右でも左でも一緒
            node->ty = lhs->ty;
        }
    }
    else if (lhs != NULL && rhs == NULL)
    {
        if (kind == ND_DEREF)
            node->ty = lhs->ty->ptr_to;
        else if (kind == ND_ADDR)
        {
            ty->ptr_to = lhs->ty;
            ty->ty = PTR;
            node->ty = ty;
        }
        else
            node->ty = lhs->ty;
    }
    else if (lhs == NULL && rhs != NULL)
    {
        node->ty = rhs->ty;
    }
    else
    {
        node->ty = NULL;
    }
    return node;
}

Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = INT;
    node->ty = ty;
    return node;
}

Node *new_node_var(int offset, Type *ty)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    node->offset = offset;
    node->ty = ty;
    return node;
}

Node *new_node_func(char *name, int length)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNC;
    node->name = name;
    node->length = length;
    Type *ty = calloc(1, sizeof(Type));
    ty->ty = INT;
    node->ty = ty;
    return node;
}

Node *num()
{
    return new_node_num(expect_number());
}

Node *ident()
{
    Token *tok = consume_ident();
    if (tok->length == 3 && strncmp(tok->string, "int", 3) == 0)
    {
        Type *ty = calloc(1, sizeof(Type));
        ty->ty = INT;
        //int * *x;
        while (consume("*"))
        {
            Type *prev = ty;
            ty = calloc(1, sizeof(Type));
            ty->ty = PTR;
            ty->ptr_to = prev;
        }
        tok = consume_ident();
        int offset = def_lvar(tok, ty);
        return new_node_var(offset, ty);
    }
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
            offset = lvar->offset;
        else
            error("変数が定義されていません");
        return new_node_var(offset, lvar->ty);
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
    else if (consume("&"))
    {
        return new_node(ND_ADDR, unary(), NULL);
    }

    else if (consume("*"))
    {

        return new_node(ND_DEREF, unary(), NULL);
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
        Node *init;
        Node *condition;
        Node *on_end;
        expect("(");
        if (consume(";"))
        {
            init = NULL;
        }
        else
        {
            init = expr();
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
            on_end = NULL;
        }
        else
        {
            on_end = expr();
            expect(")");
        }
        Node *body = stmt();
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        node->init = init;
        node->condition = condition;
        node->on_end = on_end;
        node->body = body;
    }
    else if (consume_while())
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = stmt();
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        node->condition = condition;
        node->body = body;
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

    if (tok->length == 3 && strncmp(tok->string, "int", 3) == 0)
        tok = consume_ident();
    else
        error("関数の宣言に型がありません");
    node->name = tok->string;
    node->length = tok->length;
    expect("(");
    Node *arg_top = node;
    int arg_count = 0;
    while (!consume(")"))
    {
        arg_count++;
        if (arg_count > 6)
            error("引数の数が多すぎます");
        if (arg_count != 1)
            expect(",");
        Token *arg_token = consume_ident();

        if (arg_token->length == 3 && strncmp(arg_token->string, "int", 3) == 0)
            arg_token = consume_ident();
        else
            error("引数に型がありません");
        Type *ty = calloc(1, sizeof(Type));
        ty->ty = INT;
        int offset = def_lvar(arg_token, ty);
        Node *arg = new_node_var(offset, ty);
        arg_top->lhs = arg;
        arg_top = arg;
    }
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
        locals = NULL;
    }
    nodes[i + 1] = NULL;
}

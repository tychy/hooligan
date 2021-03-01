#include "hooligan.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    if ((lhs->ty->ty == PTR || lhs->ty->ty == ARRAY) && (rhs->ty->ty == PTR || rhs->ty->ty == ARRAY))
    {
        if (kind != ND_ASSIGN)
            error("式にはintが必要です");
        // TODO 代入式の両辺の型が等しいかチェックする
        // -Wincompatible-pointer-typesというオプションらしい
        node->ty = lhs->ty;
    }
    else if (lhs->ty->ty == INT && (rhs->ty->ty == PTR || rhs->ty->ty == ARRAY))
    {
        node->ty = rhs->ty;
    }
    else if ((lhs->ty->ty == PTR || lhs->ty->ty == ARRAY) && rhs->ty->ty == INT)
    {
        node->ty = lhs->ty;
    }
    else if (lhs->ty->ty == INT && rhs->ty->ty == INT)
    {
        // 右でも左でも一緒
        node->ty = lhs->ty;
    }
    return node;
}

Node *new_node_single(NodeKind kind, Node *lhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    Type *ty = calloc(1, sizeof(Node));
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
        while (consume("*"))
        {
            Type *prev = ty;
            ty = calloc(1, sizeof(Type));
            ty->ty = PTR;
            ty->ptr_to = prev;
        }
        tok = consume_ident();
        if (consume("["))
        {
            int array_size = expect_number();

            Type *prev = ty;
            ty = calloc(1, sizeof(Type));
            ty->ty = ARRAY;
            ty->ptr_to = prev;
            ty->array_size = array_size;
            expect("]");
            int offset = def_lvar(tok, ty);
            return new_node_var(offset, ty);
        }
        else
        {
            int offset = def_lvar(tok, ty);
            return new_node_var(offset, ty);
        }
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
            Node *arg = new_node_single(ND_ARG, expr());
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

        if (consume("["))
        {
            int subset = expect_number();
            expect("]");
            Node *node = new_node(ND_ADD, new_node_var(offset, lvar->ty), new_node_num(subset));
            return new_node_single(ND_DEREF, node);
        }
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
        return new_node_single(ND_ADDR, unary());
    }
    else if (consume("*"))
    {
        return new_node_single(ND_DEREF, unary());
    }
    else if (consume_sizeof())
    {
        Node *arg = unary();
        if (arg->ty->ty == INT)
            return new_node_num(4);
        else if (arg->ty->ty == PTR)
            return new_node_num(8);
        else if (arg->ty->ty == ARRAY)
            return new_node_num(calc_bytes(arg->ty->ptr_to) * arg->ty->array_size);
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
        Node *cur = calloc(1, sizeof(Node));
        cur->kind = ND_BLOCK;
        node = cur;
        while (!consume("}"))
        {
            cur->lhs = stmt();
            cur->rhs = calloc(1, sizeof(Node));
            cur->kind = ND_BLOCK;
            cur = cur->rhs;
        }
        return node;
    }
    else if (consume_return())
    {
        node = new_node_single(ND_RETURN, expr());
        expect(";");
    }
    else if (consume_if())
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *iftrue = stmt();
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->condition = condition;
        node->body = iftrue;
        if (consume_else())
            node->on_else = stmt();
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
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNCDEF;
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

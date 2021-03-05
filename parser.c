#include "hooligan.h"

static Node *expr();
static Node *stmt();

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    if (!is_int_or_char(lhs->ty) && !is_int_or_char(rhs->ty))
    {
        error("式にはintが必要です");
    }
    else if (is_int_or_char(lhs->ty) && !is_int_or_char(rhs->ty))
    {
        node->ty = rhs->ty;
    }
    else if (!is_int_or_char(lhs->ty) && is_int_or_char(rhs->ty))
    {
        node->ty = lhs->ty;
    }
    else if (is_int_or_char(lhs->ty) && is_int_or_char(rhs->ty))
    {
        node->ty = new_type_int();
    }
    return node;
}

static Node *new_node_single(NodeKind kind, Node *lhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    Type *ty = calloc(1, sizeof(Node));
    if (kind == ND_DEREF)
    {
        if (is_int_or_char(lhs->ty))
        {
            error("pointer型である必要があります");
        }
        node->ty = lhs->ty->ptr_to;
    }
    else if (kind == ND_ADDR)
    {
        ty->ptr_to = lhs->ty;
        ty->ty = PTR;
        node->ty = ty;
    }
    else
        node->ty = lhs->ty;
}

static Node *new_node_assign(Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_ASSIGN;
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = lhs->ty;
}

static Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    node->ty = new_type_int();
    return node;
}

static Node *new_node_var(int offset, Type *ty)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_VAR;
    node->offset = offset;
    node->ty = ty;
    node->is_local = true;
    return node;
}

static Node *new_node_glob_var(Token *tok, Type *ty)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_VAR;
    node->name = tok->string;
    node->length = tok->length;
    node->ty = ty;
    node->is_local = false;
}

static Node *new_node_func(char *name, int length)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNC;
    node->name = name;
    node->length = length;
    node->ty = new_type_int();
    return node;
}

static Node *num()
{
    return new_node_num(expect_number());
}

static Node *ident()
{
    Token *tok = consume_ident();
    if (istype(tok, INT) || istype(tok, CHAR))
    {
        Type *ty;

        if (istype(tok, INT))
        {
            ty = new_type_int();
        }
        else if (istype(tok, CHAR))
        {
            ty = new_type_char();
        }
        else
        {
            error("定義されていない型です");
        }
        while (consume("*"))
        {
            ty = new_type_ptr(ty);
        }
        tok = consume_ident();
        if (consume("["))
        {
            int size = expect_number();
            ty = new_type_array(ty, size);
            expect("]");
            int offset = def_var(tok, ty, true);
            return new_node_var(offset, ty);
        }
        else
        {
            int offset = def_var(tok, ty, true);
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
        Var *lvar = find_var(tok, true);
        if (lvar)
            offset = lvar->offset;
        else
        {
            Var *gvar = find_var(tok, false);
            if (gvar)
                return new_node_glob_var(tok, gvar->ty);
            else
                error("変数が定義されていません");
        }

        return new_node_var(offset, lvar->ty);
    }
}

static Node *primary()
{
    Node *node;
    if (consume("("))
    {
        node = expr();
        expect(")");
    }
    else if (token->kind == TK_IDENT)
    {
        node = ident();
    }
    else
    {
        node = num();
    }
    if (consume("["))
    {
        Node *index = primary();
        expect("]");
        Node *mid = new_node(ND_ADD, node, index);
        return new_node_single(ND_DEREF, mid);
    }
    else
    {
        return node;
    }
}

static Node *unary()
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
        return new_node_num(calc_bytes(arg->ty));
    }
    return primary();
}

static Node *mul()
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

static Node *add()
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

static Node *relational()
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

static Node *equality()
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

static Node *assign()
{
    Node *node = equality();
    if (consume("="))
    {
        node = new_node_assign(node, assign());
    }

    return node;
}

static Node *expr()
{
    return assign();
}

static Node *stmt()
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

static Node *func(Token *ident, Type *ty)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_FUNCDEF;
    node->name = ident->string;
    node->length = ident->length;
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
        Type *arg_ty = new_type_int();
        if (istype(arg_token, INT))
        {
            while (consume("*"))
            {
                arg_ty = new_type_ptr(arg_ty);
            }

            arg_token = consume_ident();
        }
        else
            error("引数に型がありません");
        int offset = def_var(arg_token, arg_ty, true);
        Node *arg = new_node_var(offset, arg_ty);
        arg_top->lhs = arg;
        arg_top = arg;
    }
    node->rhs = stmt();
    if (locals)
        node->args_region_size = locals->offset;
    else
        node->args_region_size = 0;
    return node;
}

static Node *glob_var(Token *ident, Type *ty)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_GVARDEF;
    node->name = ident->string;
    node->length = ident->length;
    node->ty = ty;
    expect(";");
    def_var(ident, ty, false);
    return node;
}

static Node *def()
{
    Token *tok = consume_ident();
    Type *ty = new_type_int();

    if (istype(tok, INT))
    {
        while (consume("*"))
        {
            ty = new_type_ptr(ty);
        }

        tok = consume_ident();
    }
    else
    {
        error("定義式に型がありません");
    }
    Node *node;
    if (consume("("))
    {
        node = func(tok, ty);
    }
    else
    {

        if (consume("["))
        {
            int size = expect_number();
            ty = new_type_array(ty, size);
            expect("]");
        }
        node = glob_var(tok, ty);
    }

    return node;
}

void program()
{
    int i = 0;
    while (!at_eof())
    {
        Node *node = def();
        nodes[i] = node;
        i++;
        locals = NULL;
    }
    nodes[i + 1] = NULL;
}

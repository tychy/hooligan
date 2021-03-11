#include "hooligan.h"
static int new_string(char *p, int length)
{
    int strlabel;
    String *new_string = calloc(1, sizeof(String));
    new_string->length = length;
    new_string->p = p;
    if (strings)
    {
        strlabel = strings->label + 1;
    }
    else
    {
        strlabel = 0;
    }
    new_string->label = strlabel;
    new_string->next = strings;
    strings = new_string;
    return strlabel;
}

static Node *unary();
static Node *expr();
static Node *block();

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = determine_expr_type(lhs->ty, rhs->ty);
    if (!node->ty)
        error("式にはintが必要です");
    return node;
}

static Node *new_node_single(NodeKind kind, Node *child)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
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

static Node *new_node_var(Var *var)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_VAR;
    if (var->is_local)
    {
        node->offset = var->offset;
    }
    else
    {
        node->name = var->name;
        node->length = var->length;
    }
    node->ty = var->ty;
    node->is_local = var->is_local;
    return node;
}

static Node *new_node_string(int strlabel)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_STRING;
    node->strlabel = strlabel;
    node->ty = new_type_string();
    return node;
}

static Node *num()
{
    return new_node_num(expect_number());
}

static Node *ident()
{
    Token *ident = consume_ident();
    if (consume("("))
    {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_FUNC;
        node->name = ident->string;
        node->length = ident->length;
        node->ty = new_type_int(); // TODO 関数の戻り値をどこかに保存しなければならない

        Node *arg_top = node;
        int count = 0;
        while (!consume(")"))
        {
            if (count > 0)
                expect(",");
            Node *arg = new_node_single(ND_ARG, expr());
            arg_top->next = arg;
            arg_top = arg;
            count++;
        }
        return node;
    }
    else
    {
        Var *var = find_var(ident);
        if (var)
        {
            return new_node_var(var);
        }
        {
            // for sizeof
            Type *ty = find_type(ident);
            if (!ty)
            {
                error("識別子が解決できませんでした");
            }
            Node *node = calloc(1, sizeof(Node));
            node->ty = ty;
            return node;
        }
    }
}
static Member *get_struct_member(Type *ty)
{
    for (Member *mem = ty->members; mem; mem = mem->next)
        if (mem->length == token->length &&
            strncmp(mem->name, token->string, token->length) == 0)
            return mem;
    error("メンバーがありません");
}
static Node *primary()
{
    Node *node;
    if (consume("("))
    {
        node = expr();
        expect(")");
    }
    else if (token->kind == TK_NUMBER)
    {
        node = num();
    }
    else if (token->kind == TK_IDENT)
    {
        node = ident();
    }
    else
    {
        node = calloc(1, sizeof(Node));
        node->ty = consume_type();
    }
    for (;;)
    {
        if (consume("["))
        {
            Node *index = primary();
            expect("]");
            Node *mid = new_node(ND_ADD, node, index);
            node = new_node_single(ND_DEREF, mid);
            continue;
        }

        if (consume("."))
        {
            if (node->ty->ty != STRUCT)
            {
                error("構造体ではありません");
            }
            Member *mem = get_struct_member(node->ty);
            node = new_node_single(ND_MEMBER, node);
            node->member = mem;
            node->ty = mem->ty;
            token = token->next;
            continue;
        }

        if (consume("->"))
        {
            if (node->ty->ptr_to->ty != STRUCT)
            {
                error("構造体のポインタではありません");
            }

            node = new_node_single(ND_DEREF, node);
            Member *mem = get_struct_member(node->ty);
            node = new_node_single(ND_MEMBER, node);
            node->member = mem;
            node->ty = mem->ty;
            token = token->next;
            continue;
        }

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
    else if (consume_rw(TK_SIZEOF))
    {
        Node *node = unary();
        return new_node_num(calc_bytes(node->ty));
    }
    else if (token->kind == TK_STRING)
    {
        int strlabel = new_string(token->string, token->length);
        token = token->next;
        return new_node_string(strlabel);
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

static Node *init()
{
    if (consume("{"))
    {
        Node *node = new_node_single(ND_INIT, expr());
        Node *init_top = node;
        while (!consume("}"))
        {
            expect(",");
            Node *init = new_node_single(ND_INIT, expr());
            init_top->next = init;
            init_top = init;
        }
        return node;
    }
    return assign();
}

static Node *expr()
{
    return assign();
}

static Node *defl()
{
    bool is_typedef = consume_rw(TK_TYPEDEF);
    Type *ty = consume_type();
    if (ty)
    {
        Token *ident = consume_ident();
        if (consume("["))
        {
            int size = expect_number();
            ty = new_type_array(ty, size);
            expect("]");
        }
        if (is_typedef)
        {
            def_type(ident, ty, true);
            return new_node_num(0); // なにかNodeをかえさなきゃいけないので適当に返してるだけ
        }
        Var *lvar = def_var(ident, ty, true);
        Node *node = new_node_var(lvar);
        if (consume("="))
        {
            node = new_node_assign(node, init());
        }
        return node;
    }
    else
    {
        return expr();
    }
}

static Node *stmt()
{
    Node *node;
    if (consume_rw(TK_RETURN))
    {
        node = new_node_single(ND_RETURN, expr());
        expect(";");
    }
    else if (consume_rw(TK_IF))
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *iftrue = block();
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->condition = condition;
        node->body = iftrue;
        if (consume_rw(TK_ELSE))
            node->on_else = block();
    }
    else if (consume_rw(TK_FOR))
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
            init = defl();
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
        Node *body = block();
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        node->init = init;
        node->condition = condition;
        node->on_end = on_end;
        node->body = body;
    }
    else if (consume_rw(TK_WHILE))
    {
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = block();
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        node->condition = condition;
        node->body = body;
    }
    else
    {
        node = defl();
        expect(";");
    }
    return node;
}

static Node *block()
{
    if (consume("{"))
    {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        Node *cur = node;
        while (!consume("}"))
        {
            cur->statements = block();
            cur = cur->statements;
        }
        return node;
    }
    return stmt();
}

static Node *func(Token *ident, Type *ty)
{
    new_scope();
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
        Type *arg_ty = consume_type();
        if (!arg_ty)
            error("引数に型がありません");
        Token *arg_token = consume_ident();
        Var *lvar = def_var(arg_token, arg_ty, true);
        Node *arg = new_node_var(lvar);
        arg_top->lhs = arg;
        arg_top = arg;
    }
    node->rhs = block();
    node->args_region_size = offset;
    exit_scope();
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
    bool is_typedef = consume_rw(TK_TYPEDEF);
    Type *ty = consume_type();
    if (!ty)
    {
        error("定義式に型がありません");
    }
    Token *ident = consume_ident();
    if (is_typedef)
    {
        if (consume("["))
        {
            int arr_size = expect_number();
            ty = new_type_array(ty, arr_size);
            expect("]");
        }
        def_type(ident, ty, false);
        expect(";");
        return NULL;
    }
    Node *node;
    if (consume("("))
    {
        node = func(ident, ty);
    }
    else
    {

        if (consume("["))
        {
            int arr_size = expect_number();
            ty = new_type_array(ty, arr_size);
            expect("]");
        }
        node = glob_var(ident, ty);
    }

    return node;
}

void program()
{
    current_scope = calloc(1, sizeof(Scope));
    int i = 0;
    while (!at_eof())
    {
        Node *node = def();
        if (!node)
            continue;
        nodes[i] = node;
        i++;
        offset = 0;
    }
    nodes[i + 1] = NULL;
}

#include "hooligan.h"
static void logging(Node *node)
{
    if (is_verbose)
    {
        printf("Kind: %d\n", node->kind);
    }
}

static Node *new_node_raw(NodeKind kind)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    logging(node);
    return node;
}

static String *new_string(char *p, int length)
{
    int strlabel;
    String *new_string = calloc(1, sizeof(String));
    new_string->length = length;
    new_string->p = p;
    if (ctx->strings)
    {
        strlabel = ctx->strings->label + 1;
    }
    else
    {
        strlabel = 0;
    }
    new_string->label = strlabel;
    new_string->next = ctx->strings;
    ctx->strings = new_string;
    return new_string;
}

static Node *unary();
static Node *expr();
static Node *block();

static Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = new_node_raw(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    node->ty = determine_expr_type(lhs->ty, rhs->ty);
    if (!node->ty)
        error("式にはintが必要です");
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

static Node *num()
{
    return new_node_num(expect_number());
}

static Node *character()
{
    return new_node_num(expect_char());
}

static Node *ident()
{
    Token *ident = expect_ident();
    if (consume("("))
    {
        Node *node = new_node_raw(ND_FUNC);
        Var *func = find_func(ident);
        Node *arg_top = node;
        int count = 0;

        if (func)
        {
            node->name = func->name;
            node->length = func->length;
            node->ty = func->ty;
            node->num_args = func->num_args;
            node->is_static = func->is_static;
            node->has_variable_length_arguments = func->has_variable_length_arguments;
            if (node->num_args != 0 && func->arg_ty_ls[0]->ty == VOID)
            {
                node->is_void = true;
            }
            else
            {
                node->is_void = false;
            }

            while (!consume(")"))
            {
                if (count > 0)
                    expect(",");

                if (count < node->num_args || node->has_variable_length_arguments)
                {
                    if (node->is_void)
                    {
                        error("引数は予期されていません");
                    }
                    Node *arg = new_node_single(ND_ARG, expr());
                    arg_top->next = arg;
                    arg_top = arg;
                }
                else
                {
                    expr();
                }
                count++;
            }
            if (!(node->is_void) && count < node->num_args)
            {
                error("引数が少なすぎます");
            }
        }
        else
        {
            node->name = ident->string;
            node->length = ident->length;
            node->ty = new_type_int();
            while (!consume(")"))
            {
                if (count > 0)
                    expect(",");

                Node *arg = new_node_single(ND_ARG, expr());
                arg_top->next = arg;
                arg_top = arg;

                count++;
            }
        }

        return node;
    }
    else
    {
        Var *con = find_const(ident);
        if (con)
        {
            return new_node_num(con->value);
        }
        Var *var = find_var(ident);
        if (var)
        {
            return new_node_var(var);
        }
        // for sizeof
        Type *ty = find_defined_type(ident);
        if (!ty)
        {
            error("識別子が解決できませんでした");
        }
        Node *node = calloc(1, sizeof(Node));
        node->ty = ty;
        return node;
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
        Type *ty = consume_type();
        if (ty)
        {
            expect(")");
            node = expr();
            node->ty = ty;
        }
        else
        {
            node = expr();
            expect(")");
        }
    }
    else if (token->kind == TK_NUMBER)
    {
        node = num();
    }
    else if (token->kind == TK_CHARACTER)
    {
        node = character();
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
            Node *index = expr();
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

        if (consume("++"))
        {
            // インクリメントできるかのチェックがいるかも？
            // 多分int++とかするとセグフォする
            if (node->kind != ND_VAR && node->kind != ND_MEMBER)
            {
                error("インクリメントには左辺値が必要です");
            }
            Node *variable = node;
            node = new_node(ND_ADD, variable, new_node_num(1));
            node = new_node_assign(variable, node);
            node = new_node_inc(variable, node);
            return node;
        }
        if (consume("--"))
        {
            if (node->kind != ND_VAR && node->kind != ND_MEMBER)
            {
                error("デクリメントには左辺値が必要です");
            }
            Node *variable = node;
            node = new_node(ND_SUB, variable, new_node_num(1));
            node = new_node_assign(variable, node);
            node = new_node_dec(variable, node);
            return node;
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
    else if (consume("!"))
    {
        return new_node_single(ND_NOT, unary());
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
        consume("(");
        Node *node = expr();
        expect(")");
        return new_node_num(calc_bytes(node->ty));
    }
    else if (token->kind == TK_STRING)
    {
        String *s = new_string(token->string, token->length);
        token = token->next;
        return new_node_string(s);
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
        else if (consume("%"))
        {
            node = new_node(ND_MOD, node, unary());
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

static Node *logical()
{
    Node *node = equality();
    for (;;)
    {
        if (consume("&&"))
        {
            node = new_node(ND_AND, node, equality());
            node->logical_operator_label = ctx->logical_operator_label;
            ctx->logical_operator_label += 1;
        }
        else if (consume("||"))
        {
            node = new_node(ND_OR, node, equality());
            node->logical_operator_label = ctx->logical_operator_label;
            ctx->logical_operator_label += 1;
        }
        else
        {
            return node;
        }
    }
}
static Node *assign()
{
    Node *node = logical();
    if (consume("+="))
    {
        Node *rhs = new_node(ND_ADD, node, expr());
        node = new_node_assign(node, rhs);
    }
    else if (consume("-="))
    {
        Node *rhs = new_node(ND_SUB, node, expr());
        node = new_node_assign(node, rhs);
    }
    else if (consume("*="))
    {
        Node *rhs = new_node(ND_MUL, node, expr());
        node = new_node_assign(node, rhs);
    }
    else if (consume("%="))
    {
        Node *rhs = new_node(ND_MOD, node, expr());
        node = new_node_assign(node, rhs);
    }
    else if (consume("="))
    {
        Node *rhs = assign();

        if (node->ty->ty == INT && node->ty->is_const)
        {
            error("constな変数には代入できません");
        }
        node = new_node_assign(node, rhs);
    }
    return node;
}

static Node *const_expr()
{
    if (token->kind == TK_NUMBER)
    {
        return num();
    }
    else if (token->kind == TK_CHARACTER)
    {
        return character();
    }
    else if (token->kind == TK_STRING)
    {
        String *s = new_string(token->string, token->length);
        token = token->next;
        return new_node_string(s);
    }
    else
    {
        error("定数ではありません");
    }
}
static Node *expr()
{
    return assign();
}

static Node *decl_type()
{
    Type *ty = consume_type();

    if (!ty)
    {
        error("定義式に型がありません");
    }
    if (ty->ty == VOID)
    {
        error("VOID型の変数は定義できません");
    }

    if (ty->ty == STRUCT)
    {
        Token *new_name = expect_ident();
        add_defined_type(new_name, ty, true);
        return new_node_nop();
    }

    Token *new_name = expect_ident();

    if (consume("["))
    {
        int size = expect_number();
        ty = new_type_array(ty, size);
        expect("]");
    }
    add_defined_type(new_name, ty, true);
    return new_node_nop();
}

// これやばい、いいリファクタがあったら教えてほしい
static Node *defl()
{
    if (consume_rw(TK_TYPEDEF))
    {
        return decl_type();
    }
    else if (consume_rw(TK_EXTERN))
    {
        bool is_extern = consume_rw(TK_EXTERN);
        Type *ty = consume_type();

        if (!ty)
        {
            error("定義式に型がありません");
        }
        Token *ident = expect_ident();
        if (consume("["))
        {
            int size = expect_number();
            ty = new_type_array(ty, size);
            expect("]");
        }
        def_var(ident, ty, false, false);
        return new_node_nop();
    }
    else if (consume_rw(TK_STATIC))
    {
        Type *ty = consume_type();

        if (!ty)
        {
            error("定義式に型がありません");
        }
        else if (ty->ty == VOID)
        {
            error("void型の変数は定義できません");
        }
        Token *ident = expect_ident();
        if (consume("["))
        {
            int size = expect_number();
            ty = new_type_array(ty, size);
            expect("]");
        }
        int val = 0;
        if (consume("="))
        {
            val = expect_number();
        }
        Var *svar = def_static_var(ident, ty, true, val);
        return new_node_nop();
    }
    else
    {
        Type *ty = consume_type();

        if (!ty)
        {
            return expr();
        }
        else if (ty->ty == VOID)
        {
            error("void型の変数は定義できません");
        }
        if (ty->ty == STRUCT)
        {
            Token *ident = consume_ident();

            if (ident)
            {
                if (consume("["))
                {
                    int size = expect_number();
                    ty = new_type_array(ty, size);
                    expect("]");
                }
                // struct {int x;} a;
                // struct hoge {int x;} a;
                // struct hoge a;
                Var *lvar = def_var(ident, ty, true, false);
                return new_node_var(lvar);
            }
            else
            {
                // struct hoge{int x;};
                // struct {int x;};
                return new_node_nop();
            }
        }

        Token *ident = expect_ident();
        if (consume("["))
        {
            int size;
            if (consume("]"))
            {
                size = -1; // e.x. int array[] = {1, 2, 3};
            }
            else
            {
                size = expect_number();
                expect("]");
            }
            ty = new_type_array(ty, size);
        }
        if (consume("="))
        {
            if (ty->ty != ARRAY)
            {
                Var *lvar = def_var(ident, ty, true, false);
                Node *node = new_node_var(lvar);
                return new_node_assign(node, assign());
            }

            if (consume("{"))
            {
                Node *initial = new_node_single(ND_INIT, expr());
                Node *cur = initial;
                int cnt = 1;
                for (;;)
                {
                    if (consume(","))
                    {
                        if (consume("}"))
                        {
                            break;
                        }
                        else
                        {
                            if (ty->array_size != -1 && cnt >= ty->array_size)
                            {
                                expr();
                                continue;
                            }
                            cur->next = new_node_single(ND_INIT, expr());
                            cur = cur->next;
                            cnt++;
                        }
                    }
                    else
                    {
                        expect("}");
                        break;
                    }
                }
                if (ty->array_size == -1)
                {
                    ty->array_size = cnt;
                }
                else
                {
                    for (; cnt < ty->array_size; cnt++)
                    {
                        cur->next = new_node_single(ND_INIT, new_node_num(0));
                        cur = cur->next;
                    }
                }
                Var *lvar = def_var(ident, ty, true, false);
                Node *node = new_node_var(lvar);
                return new_node_assign(node, initial);
            }
            else if (token->kind == TK_STRING)
            {
                if (!(ty->ptr_to->ty == CHAR))
                {
                    error("char型の配列が必要です");
                }
                ty->array_size = token->length + 1;
                Var *lvar = def_var(ident, ty, true, false);
                Node *node = new_node_var(lvar);
                Node *initial = new_node_single(ND_INIT, new_node_num(token->string[0]));
                Node *cur = initial;
                int cnt = 1;
                for (int i = 1; i < token->length; i++)
                {
                    cur->next = new_node_single(ND_INIT, new_node_num(token->string[i]));
                    cur = cur->next;
                }
                cur->next = new_node_single(ND_INIT, new_node_num(0)); // 終端文字の挿入
                cur = cur->next;
                token = token->next;
                return new_node_assign(node, initial);
            }
            else
            {
                error("不正な初期化式です");
            }
        }
        else
        {
            Var *lvar = def_var(ident, ty, true, false);
            return new_node_var(lvar);
        }
    }
}

static Node *stmt()
{
    Node *node;
    if (consume_rw(TK_RETURN))
    {
        if (consume(";"))
        {
            node = new_node_single(ND_RETURN, new_node_num(0));
        }
        else
        {
            node = new_node_single(ND_RETURN, expr());
            expect(";");
        }
    }
    else if (consume_rw(TK_IF))
    {
        new_scope();
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = block();
        node = new_node_raw(ND_IF);
        node->condition = condition;
        node->body = body;
        if (consume_rw(TK_ELSE))
            node->on_else = block();
        node->cond_label = ctx->scope->label;
        exit_scope();
    }
    else if (consume_rw(TK_FOR))
    {
        start_loop();
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
        node = new_node_raw(ND_FOR);
        node->init = init;
        node->condition = condition;
        node->on_end = on_end;
        node->body = body;
        node->loop_label = ctx->scope->loop_label;
        end_loop();
    }
    else if (consume_rw(TK_WHILE))
    {
        start_loop();
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = block();
        node = new_node_raw(ND_WHILE);
        node->condition = condition;
        node->body = body;
        node->loop_label = ctx->scope->loop_label;
        end_loop();
    }
    else if (consume_rw(TK_SWITCH))
    {
        start_switch();
        expect("(");
        Node *condition = expr();
        expect(")");
        node = new_node_raw(ND_SWITCH);
        node->condition = condition;
        node->break_to = ctx->break_to;
        ctx->scope->current_switch = node;
        node->child = stmt();
        end_switch();
        return node;
    }
    else if (consume_rw(TK_CASE))
    {
        if (!(ctx->scope->current_switch))
        {
            error("caseの前にはswitchが必要です");
        }
        int val;
        Token *ident = consume_ident();
        if (ident)
        {
            Var *con = find_const(ident);
            if (con)
            {
                val = con->value;
            }
            else
            {
                error("予期されていないidentです\n");
            }
        }
        else
        {

            val = expect_number();
        }
        expect(":");
        Node *node = new_node_single(ND_CASE, stmt());
        node->val = val;
        node->case_label = get_unique_num();
        node->next_case = ctx->scope->current_switch->next_case;
        ctx->scope->current_switch->next_case = node;
        return node;
    }
    else if (consume_rw(TK_DEFAULT))
    {
        if (!(ctx->scope->current_switch))
        {
            error("defaultの前にはswitchが必要です");
        }
        expect(":");
        Node *node = new_node_single(ND_CASE, stmt());
        node->kind = ND_CASE;
        node->case_label = get_unique_num();
        ctx->scope->current_switch->default_case = node;
        return node;
    }
    else if (consume_rw(TK_BREAK))
    {
        node = new_node_raw(ND_BREAK);

        node->loop_label = ctx->break_to;
    }
    else if (consume_rw(TK_CONTINUE))
    {
        node = new_node_raw(ND_CONTINUE);
        node->loop_label = ctx->continue_to;
    }
    else if (consume("{"))
    {
        // スコープを作らない
        if (consume("}"))
        {
            return new_node_nop();
        }
        Node *node = new_node_raw(ND_BLOCK);
        node->statements = stmt();
        Node *cur = node->statements;
        while (!consume("}"))
        {
            cur->next_stmt = stmt();
            cur = cur->next_stmt;
        }
        return node;
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
        if (consume("}"))
        {
            return new_node_nop();
        }
        new_scope();
        Node *node = new_node_raw(ND_BLOCK);
        node->statements = block();
        Node *cur = node->statements;
        while (!consume("}"))
        {
            cur->next_stmt = block();
            cur = cur->next_stmt;
        }
        exit_scope();
        return node;
    }
    return stmt();
}

static Node *func(Token *ident, Type *ty, bool is_static)
{
    new_scope();
    Node *node = new_node_raw(ND_FUNCDEF);
    node->name = ident->string;
    node->length = ident->length;
    Node *arg_top = node;
    Type *arg_ty_ls[6];
    int arg_idx = 0;
    while (!consume(")"))
    {
        if (arg_idx >= 6)
            error("引数の数が多すぎます");
        if (arg_idx != 0)
            expect(",");

        if (consume("..."))
        {
            if (arg_idx == 0)
            {
                error("最低一つの仮引数が必要です");
            }
            node->has_variable_length_arguments = true;
            // 可変長引数の場合、rdi~r9の6つのレジスタの中身とメタ情報をrbpの下に並べる必要があるのでその領域(8×6+8+8+4+4)を確保する
            ctx->offset = 72;
            expect(")");
            break;
        }
        Type *arg_ty = consume_type();

        if (!arg_ty)
        {
            printf("%s\n", token->string);
            error("引数に型がありません");
        }
        else if (arg_ty->ty != VOID)
        {
            Token *arg_token = expect_ident();
            if (consume("["))
            {
                int size = expect_number();
                arg_ty = new_type_ptr(arg_ty);
                expect("]");
            }

            Var *lvar = def_var(arg_token, arg_ty, true, false);
            Node *arg = new_node_var(lvar);
            arg_top->lhs = arg;
            arg_top = arg;
        }
        arg_ty_ls[arg_idx] = arg_ty;
        arg_idx++;
    }
    def_func(ident, ty, arg_idx, arg_ty_ls, is_static, node->has_variable_length_arguments);
    if (consume(";"))
    {
        exit_scope();
        return new_node_nop();
    }

    node->rhs = block();
    node->args_region_size = ctx->offset;
    node->is_static = is_static;
    exit_scope();
    return node;
}

static Node *glob_var(Token *ident, Type *ty, bool is_static)
{
    Node *node = new_node_raw(ND_GVARDEF);
    node->name = ident->string;
    node->length = ident->length;
    node->ty = ty;
    node->is_static = is_static;
    if (ty->ty == VOID)
    {
        error("void型の変数は定義できません");
    }
    def_var(ident, ty, false, is_static);

    if (consume("="))
    {
        if (ty->ty != ARRAY)
        {
            node->gvar_init = const_expr();
        }
        else if (consume("{"))
        {
            Node *initial = new_node_single(ND_INIT, const_expr());
            Node *cur = initial;
            int cnt = 1;
            for (;;)
            {
                if (consume(","))
                {
                    if (consume("}"))
                    {
                        break;
                    }
                    else
                    {
                        if (ty->array_size != -1 && cnt >= ty->array_size)
                        {
                            const_expr();
                            continue;
                        }
                        cur->next = new_node_single(ND_INIT, const_expr());
                        cur = cur->next;
                        cnt++;
                    }
                }
                else
                {
                    expect("}");
                    break;
                }
            }
            if (ty->array_size == -1)
            {
                ty->array_size = cnt;
            }
            node->gvar_init = initial;
        }
    }
    expect(";");
    return node;
}

static Node *def()
{
    Node *node;
    if (consume_rw(TK_TYPEDEF))
    {
        node = decl_type();
        expect(";");
        return node;
    }
    bool is_extern = false;
    bool is_static = false;
    if (consume_rw(TK_EXTERN))
    {
        is_extern = true;
    }
    else if (consume_rw(TK_STATIC))
    {
        is_static = true;
    }

    Type *ty = consume_type();

    if (!ty)
    {
        error("定義式に型がありません");
    }
    if (ty->ty == STRUCT)
    {
        Token *ident = consume_ident();
        if (ident)
        {
            // unused
            // struct {int x;} a;
            // struct hoge {int x;} a;
            // struct hoge a;
            if (consume("["))
            {
                int size = expect_number();
                ty = new_type_array(ty, size);
                expect("]");
            }

            return glob_var(ident, ty, is_static);
        }
        else
        {
            // struct hoge{int x;};
            // struct {int x;};
            expect(";");
            return new_node_nop();
        }
    }

    Token *ident = expect_ident();
    if (consume("("))
    {
        node = func(ident, ty, is_static);
    }
    else
    {

        if (consume("["))
        {
            int arr_size = expect_number();
            ty = new_type_array(ty, arr_size);
            expect("]");
        }
        node = glob_var(ident, ty, is_static);
        if (is_extern)
        {
            return new_node_nop();
        }
    }

    return node;
}

void program()
{
    if (is_verbose)
    {
        printf("\x1b[33mSTART PARSING\x1b[0m\n");
    }

    ctx = calloc(1, sizeof(Context));
    ctx->scope = calloc(1, sizeof(Scope));
    int i = 0;
    while (!at_eof())
    {
        Node *node = def();
        nodes[i] = node;
        i++;
        ctx->offset = 0;
    }
    nodes[i + 1] = NULL;
    if (is_verbose)
    {
        printf("\x1b[33mEND PARSING\x1b[0m\n");
    }
}

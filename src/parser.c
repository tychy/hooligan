#include "hooligan.h"

#include "parser/logging.c"
#include "parser/type.c"
#include "parser/variable.c"
#include "parser/read_token.c"
#include "parser/scope.c"
#include "parser/node_constructor.c"

#define MAX_NODES 500
static Node *unary();
static Node *expr();
static Node *block();
static Node *def(bool is_global);

static Node *num()
{
    if (cur_token->is_float)
    {
        return new_node_float(expect_float());
    }
    return new_node_num(expect_number());
}

static Node *character()
{
    return new_node_num(expect_char());
}

static Node *string()
{
    return new_node_string(expect_string());
}

static Node *ident()
{
    Token *ident = expect_ident();
    if (consume("("))
    {
        Node *node = new_node_raw(ND_FUNC);
        Var *func = find_var(ident);
        int count = 0;

        if (func)
        {
            node->name = func->name;
            node->length = func->length;
            node->ty = func->ty;
            node->is_static = func->is_static;
            node->has_variable_length_arguments = func->has_variable_length_arguments;
            int args_count = func->num_args;
            if (args_count != 0 && func->arg_ty_ls[0]->ty == VOID)
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

                if (count < args_count || node->has_variable_length_arguments)
                {
                    if (node->is_void)
                    {
                        error("引数は予期されていません");
                    }
                    node->args = append(node->args, new_node_single(ND_ARG, expr()));
                }
                else
                {
                    expr();
                }
                count++;
            }
            if (!(node->is_void) && count < args_count)
            {
                error("引数が少なすぎます");
            }
        }
        else
        {
            node->name = ident->str;
            node->length = ident->len;
            node->ty = new_type_int();
            while (!consume(")"))
            {
                if (count > 0)
                    expect(",");

                node->args = append(node->args, new_node_single(ND_ARG, expr()));
                count++;
            }
        }

        return node;
    }
    else
    {
        Var *var = find_var(ident);
        if (var)
        {
            if (var->is_computable)
            {
                return new_node_num(var->value);
            }
            else
            {
                return new_node_var(var);
            }
        }
        // for sizeof
        Type *ty = find_defined_type(ident);
        if (!ty)
        {
            error_at(ident->str, "識別子が解決できませんでした");
        }
        while (consume("*"))
        {
            ty = new_type_ptr(ty);
        }
        Node *node = new_node_raw(ND_TYPE);
        node->ty = ty;
        return node;
    }
}
static Member *get_struct_member(Type *ty)
{
    for (Member *mem = ty->members; mem; mem = mem->next)
        if (mem->length == cur_token->len &&
            strncmp(mem->name, cur_token->str, cur_token->len) == 0)
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
    else if (cur_token->kind == TK_NUMBER)
    {
        node = num();
    }
    else if (cur_token->kind == TK_CHARACTER)
    {
        node = character();
    }
    else if (cur_token->kind == TK_IDENT)
    {
        node = ident();
    }
    else
    {
        node = calloc(1, sizeof(Node));
        node->kind = ND_TYPE;
        node->ty = consume_type();
    }
    for (;;)
    {
        if (consume("["))
        {
            Node *index = expr();
            expect("]");
            node = new_node(ND_ADD, node, index);
            while (node->ty->ptr_to && node->ty->ptr_to->ty == ARRAY && consume("["))
            {
                Node *index = expr();
                expect("]");
                node = new_node(ND_ADD, node, index);
                node->ty = node->ty->ptr_to;
            }
            node = new_node_single(ND_DEREF, node);
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
            cur_token = cur_token->next;
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
            cur_token = cur_token->next;
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
    else if (consume_rw(RW_SIZEOF))
    {
        consume("(");
        Node *node = expr();
        expect(")");
        return new_node_num(calc_bytes(node->ty));
    }
    else if (cur_token->kind == TK_STRING)
    {
        return string();
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
            node = new_node(ND_GEQ, add(), node);
        }
        else if (consume(">"))
        {
            node = new_node(ND_GTH, node, add());
        }
        else if (consume("<"))
        {
            node = new_node(ND_GTH, add(), node);
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
            node = new_node_single(ND_NOT, new_node(ND_EQUAL, node, relational()));
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
        }
        else if (consume("||"))
        {
            node = new_node(ND_OR, node, equality());
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
    else if (consume("/="))
    {
        Node *rhs = new_node(ND_DIV, node, expr());
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
    if (cur_token->kind == TK_NUMBER)
    {
        return num();
    }
    else if (cur_token->kind == TK_CHARACTER)
    {
        return character();
    }
    else if (cur_token->kind == TK_STRING)
    {
        return string();
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

static Node *right_value(Type *lty)
{
    Node *node = NULL;
    if (consume("="))
    {
        Node head;
        Node *cur = &head;
        if (lty->ty != ARRAY)
        {
            cur->next = assign();
        }
        else if (consume("{"))
        {
            int cnt = 0;
            while (!consume("}"))
            {
                if (lty->array_size != -1 && cnt >= lty->array_size)
                {
                    expr();
                    consume(",");
                    continue;
                }
                cur->next = new_node_single(ND_INIT, expr());
                cur = cur->next;
                cnt++;
                consume(",");
            }
            if (lty->array_size == -1)
            {
                if (cnt == 0)
                {
                    error("配列のサイズが決定されていません");
                }
                lty->array_size = cnt;
            }
            for (; cnt < lty->array_size; cnt++)
            {
                cur->next = new_node_single(ND_INIT, new_node_num(0));
                cur = cur->next;
            }
        }
        else if (cur_token->kind == TK_STRING)
        {
            if (!(lty->ptr_to->ty == CHAR))
            {
                error("char型の配列が必要です");
            }
            lty->array_size = cur_token->len + 1;
            for (int i = 0; i < cur_token->len; i++)
            {
                cur->next = new_node_single(ND_INIT, new_node_num(cur_token->str[i]));
                cur = cur->next;
            }
            cur->next = new_node_single(ND_INIT, new_node_num(0)); // 終端文字の挿入
            cur_token = cur_token->next;
        }
        node = head.next;
    }
    return node;
}

static Node *stmt()
{
    Node *node;
    if (consume_rw(RW_RETURN))
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
    else if (consume_rw(RW_IF))
    {
        new_scope();
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = block();
        node = new_node_raw(ND_IF);
        node->condition = condition;
        node->body = body;
        if (consume_rw(RW_ELSE))
            node->on_else = block();
        exit_scope();
    }
    else if (consume_rw(RW_FOR))
    {
        new_scope();
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
            init = def(true);
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
        node = new_node_raw(ND_FOR);
        // TODO リファクタ
        int prev_break_to = ctx->break_to;
        int prev_continue_to = ctx->continue_to;
        ctx->break_to = node->id;
        ctx->continue_to = node->id;
        Node *body = block();
        node->init = init;
        node->condition = condition;
        node->on_end = on_end;
        node->body = body;
        ctx->break_to = prev_break_to;
        ctx->continue_to = prev_continue_to;
        exit_scope();
    }
    else if (consume_rw(RW_WHILE))
    {
        new_scope();
        node = new_node_raw(ND_WHILE);
        // TODO リファクタ
        int prev_break_to = ctx->break_to;
        int prev_continue_to = ctx->continue_to;
        ctx->break_to = node->id;
        ctx->continue_to = node->id;
        expect("(");
        Node *condition = expr();
        expect(")");
        Node *body = block();
        node->condition = condition;
        node->body = body;
        ctx->break_to = prev_break_to;
        ctx->continue_to = prev_continue_to;
        exit_scope();
    }
    else if (consume_rw(RW_SWITCH))
    {
        new_scope();
        node = new_node_raw(ND_SWITCH);
        int prev_break_to = ctx->break_to;
        ctx->break_to = node->id;
        expect("(");
        Node *condition = expr();
        expect(")");
        node->condition = condition;
        ctx->scope->current_switch = node;
        node->child = stmt();
        ctx->break_to = prev_break_to;
        exit_scope();
        return node;
    }
    else if (consume_rw(RW_CASE))
    {
        if (!(ctx->scope->current_switch))
        {
            error("caseの前にはswitchが必要です");
        }
        int val;
        Token *ident = consume_ident();
        if (ident)
        {
            Var *var = find_var(ident);
            if (var->is_computable)
            {
                val = var->value;
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
        node->next_case = ctx->scope->current_switch->next_case;
        ctx->scope->current_switch->next_case = node;
        return node;
    }
    else if (consume_rw(RW_DEFAULT))
    {
        if (!(ctx->scope->current_switch))
        {
            error("defaultの前にはswitchが必要です");
        }
        expect(":");
        Node *node = new_node_single(ND_CASE, stmt());
        node->kind = ND_CASE;
        ctx->scope->current_switch->default_case = node;
        return node;
    }
    else if (consume_rw(RW_BREAK))
    {
        node = new_node_raw(ND_BREAK);
        node->break_to_id = ctx->break_to;
    }
    else if (consume_rw(RW_CONTINUE))
    {
        node = new_node_raw(ND_CONTINUE);
        node->continue_to_id = ctx->continue_to;
    }
    else if (consume("{"))
    {
        // スコープを作らない
        if (consume("}"))
        {
            return new_node_nop();
        }
        Node *node = new_node_raw(ND_BLOCK);
        while (!consume("}"))
        {
            node->statements = append(node->statements, stmt());
        }
        return node;
    }
    else
    {
        node = def(true);
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
        while (!consume("}"))
        {
            node->statements = append(node->statements, block());
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
    node->name = ident->str;
    node->length = ident->len;
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
            ctx->offset = 80;
            expect(")");
            break;
        }
        Type *arg_ty = consume_type();

        if (!arg_ty)
        {
            printf("%s\n", cur_token->str);
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

    // TODO グローバルスコープに関数を登録するためのHACK、もうちょっといい方法を考える
    ctx->scope = ctx->scope->prev;
    def_func(ident, ty, arg_idx, arg_ty_ls, is_static, node->has_variable_length_arguments);
    ctx->scope = ctx->scope->next;

    if (consume(";"))
    {
        node = new_node_nop();
    }
    else
    {
        node->rhs = block();
        node->args_region_size = ctx->offset;
        node->is_static = is_static;
    }
    exit_scope();
    return node;
}

static Node *def(bool is_global)
{
    Node *node = NULL;
    if (consume_rw(RW_TYPEDEF))
    {
        node = decl_type();
        expect(";");
        return node;
    }

    bool is_extern = consume_rw(RW_EXTERN);
    bool is_static = consume_rw(RW_STATIC);
    is_extern = is_extern || consume_rw(RW_EXTERN);
    is_static = is_static || consume_rw(RW_STATIC);
    if (is_extern && is_static)
    {
        error("externはnon-staticな文脈でのみ使用できます");
    }

    Type *ty = consume_type();
    if (!ty)
    {
        if (is_static || is_extern)
        {
            error("定義式に型がありません");
        }
        else
        {
            node = expr();
            expect(";");
            return node;
        }
    }

    Token *ident = consume_ident();
    if (!ident)
    {
        if (ty->ty == STRUCT)
        {
            // struct hoge{int x;};
            // struct {int x;};
            expect(";");
            return new_node_nop();
        }
        ident = expect_ident();
    }
    int array_dimension = 0;
    int array_sizes[10]; // TODO 10次元以上の配列を作れるようにする
    while (consume("["))
    {
        int array_size = 0;
        array_dimension++;
        if (consume("]"))
        {
            if (array_dimension == 1)
            {
                array_size = -1;
            }
            else
            {
                error("array has incomplete element type");
            }
        }
        else
        {
            array_size = expect_number();
            expect("]");
        }
        array_sizes[array_dimension - 1] = array_size;
    }
    for (int i = 0; i < array_dimension; i++)
    {
        ty = new_type_array(ty, array_sizes[array_dimension - i - 1]);
    }

    if (consume("("))
    {
        node = func(ident, ty, is_static);
    }
    else
    {
        Node *rval = right_value(ty); // TODO 右辺値がコンパイル時に計算可能か求める手段を用意する
        Var *var = def_var(ident, ty, is_global && !is_extern, is_static);
        node = new_node_var(var);
        if (is_static || !is_global)
        {
            node->kind = ND_GVARDEF;
            node->gvar_init = rval;
        }
        else if (rval)
        {
            node = new_node_assign(node, rval);
        }
        expect(";");
        if (is_extern)
        {
            return new_node_nop();
        }
    }
    return node;
}

Node **parse_program(Token *start)
{
    if (opts->is_verbose)
    {
        printf("\x1b[33mSTART PARSING\x1b[0m\n");
    }

    Node **nodes = calloc(MAX_NODES, sizeof(Node *));
    cur_token = start;
    int i = 0;
    while (!at_eof())
    {
        if (i >= MAX_NODES)
        {
            error("ノードが多すぎます");
        }
        Node *node = def(false);
        nodes[i] = node;
        i++;
        ctx->offset = 0;
    }
    nodes[i + 1] = NULL;
    if (opts->is_verbose)
    {
        printf("\x1b[33mEND PARSING\x1b[0m\n");
    }
    return nodes;
}

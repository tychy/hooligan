#include "../hooligan.h"

static void set_struct_member(Type *ty);
static Type *consume_type();

static bool consume(char *op)
{
    if (token->kind != TK_OPERATOR || strcmp(token->string, op))
        return false;
    token = token->next;
    return true;
}

static bool consume_rw(TokenKind tk)
{
    if (token->kind != tk)
        return false;
    token = token->next;
    return true;
}

static void expect(char *op)
{
    if (token->kind != TK_OPERATOR || token->string[0] != op[0])
    {
        error2("'%s'ではありません, got %s", op, token->string);
    }
    token = token->next;
}

static int expect_number()
{
    if (token->kind != TK_NUMBER)
        error_at(token->string, "数字ではありません");
    int value = token->value;
    token = token->next;
    return value;
}

static float expect_float()
{
    if (token->kind != TK_NUMBER)
        error_at(token->string, "数字ではありません");
    if (!token->is_float)
        error_at(token->string, "floatではありません");

    float value = token->float_val;
    token = token->next;
    return value;
}

static int expect_char()
{
    if (token->kind != TK_CHARACTER)
    {
        error_at(token->string, "文字ではありません");
    }
    int value = token->value;
    token = token->next;
    return value;
}

static Token *consume_ident()
{
    if (token->kind != TK_IDENT)
    {
        return NULL;
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

static Token *expect_ident()
{
    if (token->kind != TK_IDENT)
    {
        error1("識別子ではありません got %s", token->string);
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

static bool at_eof()
{
    return token->kind == TK_EOF;
}

static void set_struct_member(Type *ty)
{
    int offset = 0;
    Member *head = calloc(1, sizeof(Member));
    Member *cur = head;
    while (!(consume("}")))
    {

        Member *mem = calloc(1, sizeof(Member));
        Type *mem_ty = consume_type();

        Token *mem_tok = consume_ident();
        if (consume("["))
        {
            int arr_size = expect_number();
            mem_ty = new_type_array(mem_ty, arr_size);
            expect("]");
        }

        mem->name = mem_tok->string;
        mem->length = mem_tok->length;
        mem->offset = offset;
        offset += calc_bytes(mem_ty);
        mem->ty = mem_ty;
        cur->next = mem;
        cur = mem;
        expect(";");
    }
    ty->members = head;
    ty->size = offset;
}

static Type *consume_type()
{
    bool is_const = false;
    if (consume_rw(TK_CONST))
    {
        is_const = true;
    }

    Type *ty = find_defined_type(token);
    if (ty)
    {
        token = token->next;

        while (consume("*"))
        {
            ty = new_type_ptr(ty);
        }
        return ty;
    }
    if (consume_rw(TK_INT))
    {
        ty = new_type_int();
    }
    else if (consume_rw(TK_FLOAT))
    {
        ty = new_type_float();
    }
    else if (consume_rw(TK_CHAR))
    {
        ty = new_type_char();
    }
    else if (consume_rw(TK_STRUCT))
    {
        ty = new_type_struct();
    }
    else if (consume_rw(TK_VOID))
    {
        ty = new_type_void();
    }
    else if (consume_rw(TK_ENUM))
    {
        ty = new_type_int();
        if (consume("{"))
        {
            int i = 0;
            while (!(consume("}")))
            {
                Token *ident = consume_ident();
                def_const(ident, i);
                i++;
                if (!(consume(",")))
                {
                    expect("}");
                    break;
                }
            }
            if (i == 0)
            {
                error_at(token->string, "識別子が必要です");
            }
        }
        else
        {
            error_at(token->string, "識別子が必要です");
        }
    }
    else
    {
        return NULL;
    }

    if (ty->ty == STRUCT)
    {
        Token *ident = consume_ident();
        if (consume("{"))
        {
            if (ident)
            {
                Type *tagged_type = find_tagged_type(ident);
                if (tagged_type)
                {
                    ty = tagged_type;
                }
                else
                {
                    add_tagged_type(ident, ty, true);
                }
            }
            set_struct_member(ty);
        }
        else if (ty->size == -1)
        {
            if (ident)
            {
                // typedef struct f ma;
                // struct f{ int x;}
                // struct f ma;
                Type *tagged_type = find_tagged_type(ident);
                if (tagged_type)
                {
                    ty = tagged_type;
                }
                else
                {
                    add_tagged_type(ident, ty, true);
                }
            }
        }
    }
    if (consume_rw(TK_CONST))
    {
        is_const = is_const || true;
    }
    ty->is_const = is_const;

    while (consume("*"))
    {
        ty = new_type_ptr(ty);
    }

    if (consume_rw(TK_CONST))
    {
        ty->is_const = true;
    }
    return ty;
}

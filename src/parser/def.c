#include "../hooligan.h"

static Node *def()
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

    if (consume("("))
    {
        node = func(ident, ty, is_static);
    }
    else
    {
        node = glob_var(ident, ty, is_static);
        expect(";");
        if (is_extern)
        {
            return new_node_nop();
        }
    }

    return node;
}

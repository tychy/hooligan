#include "../../tokenizer2.c"
#include "../../util.c"
PPContext *pp_ctx;

void dump_pp_token(PPToken *tok);

void testPPToken()
{
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/1.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/2.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/3.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/4.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/5.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/6.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/7.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/8.c")));
    return;
}

void testMacro()
{
    pp_ctx = calloc(1, sizeof(PPContext));
    pp_ctx->macros = NULL;
    dump_pp_token(preprocess_directives(decompose_to_pp_token(read_file("unittest/tokenizer2/include/5.c"))));

    pp_ctx = calloc(1, sizeof(PPContext));
    pp_ctx->macros = NULL;
    dump_pp_token(preprocess_directives(decompose_to_pp_token(read_file("unittest/tokenizer2/include/6.c"))));

    pp_ctx = calloc(1, sizeof(PPContext));
    pp_ctx->macros = NULL;
    dump_pp_token(preprocess_directives(decompose_to_pp_token(read_file("unittest/tokenizer2/include/7.c"))));

    pp_ctx = calloc(1, sizeof(PPContext));
    pp_ctx->macros = NULL;
    dump_pp_token(preprocess_directives(decompose_to_pp_token(read_file("unittest/tokenizer2/include/8.c"))));

    return;
}
int main()
{

    testPPToken();
    testMacro();
    return 0;
}

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

    return;
}

void testMacro()
{
    pp_ctx = calloc(1, sizeof(PPContext));
    dump_pp_token(preprocess_macro(decompose_to_pp_token(read_file("unittest/tokenizer2/include/5.c"))));
    return;
}
int main()
{

    testPPToken();
    testMacro();
    return 0;
}

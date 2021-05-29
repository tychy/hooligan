#include "../../tokenizer2.c"
#include "../../util.c"

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

void testInclude()
{
}
int main()
{
    testPPToken();
    return 0;
}

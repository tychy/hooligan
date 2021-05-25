#include "../../tokenizer2.c"
#include "../../util.c"

void dump_pp_token(PPToken *tok);

int main()
{
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/1.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/2.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/3.c")));
    return 0;
}

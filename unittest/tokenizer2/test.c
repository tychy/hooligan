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
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/9.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/10.c")));
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/include/11.c")));

    return;
}

void run_preprocess(char *str)
{
    char *dir = extract_dir(str);
    pp_ctx = calloc(1, sizeof(PPContext));
    pp_ctx->macros = NULL;
    dump_pp_token(preprocess_directives(dir, decompose_to_pp_token(read_file(str))));
    return;
}
void testMacro()
{

    //run_preprocess("unittest/tokenizer2/include/3.c"); <-ユニットテストかけない　出力が長すぎることとhooligan.hのifndef endifが読めない
    run_preprocess("unittest/tokenizer2/include/5.c");
    run_preprocess("unittest/tokenizer2/include/6.c");
    run_preprocess("unittest/tokenizer2/include/7.c");
    run_preprocess("unittest/tokenizer2/include/8.c");

    run_preprocess("unittest/tokenizer2/include/10.c");
    run_preprocess("unittest/tokenizer2/include/11.c");
    return;
}
int main()
{

    testPPToken();
    testMacro();
    return 0;
}

#include "../../src/preprocessor.c"
#include "../../src/util.c"

void dump_pp_token(PPToken *tok);

void testPPToken()
{
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/1.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/2.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/3.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/4.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/5.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/6.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/7.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/8.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/9.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/10.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/11.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/12.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/13.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/14.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/15.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/16.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/17.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/18.c")));
    dump_pp_token(decompose_to_pp_token(read_file("tests/preprocessor/include/19.c")));
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

    run_preprocess("tests/preprocessor/include/5.c");
    run_preprocess("tests/preprocessor/include/6.c");
    run_preprocess("tests/preprocessor/include/7.c");
    run_preprocess("tests/preprocessor/include/8.c");

    run_preprocess("tests/preprocessor/include/10.c");
    run_preprocess("tests/preprocessor/include/11.c");

    run_preprocess("tests/preprocessor/include/13.c");
    run_preprocess("tests/preprocessor/include/14.c");
    run_preprocess("tests/preprocessor/include/15.c");
    run_preprocess("tests/preprocessor/include/16.c");
    run_preprocess("tests/preprocessor/include/17.c");
    run_preprocess("tests/preprocessor/include/18.c");
    run_preprocess("tests/preprocessor/include/19.c");
    return;
}
int main()
{

    testPPToken();
    testMacro();
    return 0;
}

#include "../../hooligan.h"

typedef enum
{
    PPTK_HN, // header-name
    PPTK_IDENT,
    PPTK_NUMBER,
    PPTK_CHAR, // character-constant
    PPTK_STRING,
    PPTK_PUNC, // punctuators
} PPTokenKind;

typedef struct PPToken PPToken;

struct PPToken
{
    PPTokenKind kind;
    PPToken *next;
    int val;
    int len;
    char *str;
};

PPToken *decompose_to_pp_token(char *p);
void dump_pp_token(PPToken *tok);

int main()
{
    dump_pp_token(decompose_to_pp_token(read_file("unittest/tokenizer2/sample.c")));
    return 0;
}

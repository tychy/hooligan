#ifndef HOOLIGAN_H
#define HOOLIGAN_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
typedef enum
{
    PPTK_DUMMY, // #define identで使う
    PPTK_HN,    // header-name
    PPTK_IDENT,
    PPTK_NUMBER,
    PPTK_CHAR, // character-constant
    PPTK_STRING,
    PPTK_PUNC, // punctuators
} PPTokenKind;

// NOTE: 予約語を先頭に持ってくる
typedef enum
{
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_FOR,
    TK_WHILE,
    TK_SWITCH,
    TK_CASE,
    TK_DEFAULT,
    TK_SIZEOF,
    TK_VOID,
    TK_INT,
    TK_CHAR,
    TK_STRUCT,
    TK_TYPEDEF,
    TK_BREAK,
    TK_CONTINUE,
    TK_STATIC,
    TK_EXTERN,
    TK_ENUM,
    TK_CONST,
    // add reserved word above
    TK_OPERATOR,
    TK_NUMBER,
    TK_IDENT,
    TK_EOF,
    TK_STRING,
    TK_CHARACTER,
} TokenKind;

typedef enum
{
    ND_NUM,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
    ND_EQUAL,
    ND_NEQUAL,
    ND_GEQ,
    ND_LEQ,
    ND_GTH,
    ND_LTH,
    ND_VAR,
    ND_ASSIGN,
    ND_RETURN,
    ND_IF,
    ND_FOR,
    ND_BLOCK,
    ND_WHILE,
    ND_SWITCH,
    ND_CASE,
    ND_FUNC,
    ND_ARG,
    ND_FUNCDEF,
    ND_ADDR,
    ND_DEREF,
    ND_GVARDEF,
    ND_STRING,
    ND_INIT,
    ND_MEMBER,
    ND_BREAK,
    ND_CONTINUE,
    ND_AND,
    ND_OR,
    ND_NOT,
    ND_POSTINC,
    ND_POSTDEC,
    ND_NOP,
    ND_TYPE,
} NodeKind;

typedef enum
{
    VOID,
    INT,
    CHAR,
    PTR,
    ARRAY,
    STRUCT,
    ENUM,
} TypeKind;

typedef enum
{
    RG_RDI,
    RG_RSI,
    RG_RDX,
    RG_RCX,
    RG_R8,
    RG_R9,
    RG_RAX,
    RG_RBP,
    RG_RSP,
} RegisterName;

// type definition
typedef struct Option Option;
typedef struct PPToken PPToken;
typedef struct Macro Macro;
typedef struct PPContext PPContext;
typedef struct Token Token;
typedef struct Type Type;
typedef struct Node Node;
typedef struct Var Var;
typedef struct String String;
typedef struct Member Member;
typedef struct Scope Scope;
typedef struct Context Context;

struct Option
{
    bool is_verbose;
};

struct PPToken
{
    PPTokenKind kind;
    PPToken *next;
    int val;
    int len;
    char *str;
};

struct Macro
{
    PPToken *target;
    PPToken *replace;
    Macro *next;
};
struct PPContext
{
    Macro *macros;
};

struct Token
{
    TokenKind kind;
    Token *next;
    int value;
    int length;
    char *string;
};

struct Type
{
    TypeKind ty;
    Type *ptr_to;
    size_t array_size;
    int size; // structで使う
    Member *members;

    // for defined type
    char *name;
    int length;
    Type *next_defined;

    // for struct
    char *tag_name;
    int tag_length;
    Type *next_tagged;

    // for const variable
    bool is_const;
};

struct Node
{
    NodeKind kind;
    int val;
    int offset;
    char *name;
    int length;
    Type *ty;

    // for gvar
    Node *gvar_init;

    // for binary tree node
    Node *lhs;
    Node *rhs;
    // for single child node
    Node *child;
    // for
    Node *next;

    // for(init; condition; on_end) body;
    Node *init;
    Node *condition;
    Node *on_end;
    Node *body;

    // if(condition) body else on_else
    Node *on_else;
    // for switch-case
    Node *next_case;
    Node *default_case;
    // for variable
    bool is_local;
    // for static
    bool is_static;
    int scope_label;
    // for function
    int args_region_size;
    Node *args;
    int num_args;
    bool is_void;
    bool has_variable_length_arguments;

    // for string
    int strlabel;

    //for struct
    Member *member;

    //for break in switch
    int break_to;

    // labels
    int loop_label;             // for, while
    int cond_label;             // if, else
    int case_label;             // switch-case
    int logical_operator_label; // &&, ||

    Node *statements; // for block
    Node *next_stmt;  // for block children
};
struct Var
{
    char *name;
    int length;
    int offset;
    int label;
    Type *ty;
    Var *next;
    bool is_local;
    bool is_typedef;
    // for static variable
    bool is_static;
    int init_val;
    // for function
    bool is_function;
    int num_args;
    Type *arg_ty_ls[6];
    bool has_variable_length_arguments;
    // for constant expression
    int value;
    // for const variable
    bool is_const;
};

struct String
{
    char *p;
    int length;
    int label;
    String *next;
};

struct Member
{
    Member *next;
    char *name;
    Type *ty;
    int length;
    int offset;
};

struct Scope
{
    Var *variables;
    Var *constants;
    Type *defined_type; // for typedef
    Type *tagged_types; // for tag(e.x. struct)
    Scope *prev;
    Scope *next;
    Node *current_switch;
    int label;
    int loop_label; // for break and continue
    int break_to;
};

struct Context
{
    int scope_serial_num; // serial number for scope
    Scope *scope;
    String *strings;
    Var *statics;
    Var *functions;
    int offset; // for local variable
    int break_to;
    int continue_to;
    int logical_operator_label;
};

// Declaration of global variables
extern Option *opts;
extern PPContext *pp_ctx;
extern Token *token;
extern Node *nodes[500];
extern Context *ctx;
extern FILE *output;

// Declaration of functions
// read_token.c
bool consume(char *op);
bool consume_rw(TokenKind tk);
Type *consume_type();
Type *consume_ptr(Type *ty);
void expect(char *op);
int expect_number();
int expect_char();
bool at_eof();
Token *consume_ident();
Token *expect_ident();

// tokenizer.c
Token *tokenize(PPToken *pp_tok);

// preprcessor.c
PPToken *decompose_to_pp_token(char *p);
PPToken *preprocess_directives(char *base_dir, PPToken *tok);

// parser.c
void program();

// generator.c
void gen_asm_intel();

// variable.c
Var *find_var(Token *tok);
Var *def_var(Token *tok, Type *ty, bool is_local, bool is_static);
Var *def_static_var(Token *tok, Type *ty, bool is_local, int init_val);
Var *find_func(Token *tok);
Var *def_func(Token *tok, Type *ty, int num_args, Type *arg_ty_ls[6], bool is_static, bool has_variable_length_arguments);
Var *find_const(Token *tok);
Var *def_const(Token *tok, int val);

// type.c
Type *new_type_int();
Type *new_type_char();
Type *new_type_string();
Type *new_type_void();
Type *new_type_ptr(Type *ptr_to);
Type *new_type_array(Type *ptr_to, size_t size);
Type *new_type_struct();
bool is_int(Type *ty);
bool is_char(Type *ty);
bool is_int_or_char(Type *ty);
bool is_ptr(Type *ty);
int calc_bytes(Type *ty);
Type *determine_expr_type(Type *lhs, Type *rhs);
Type *add_defined_type(Token *tok, Type *ty, bool is_local);
Type *add_tagged_type(Token *tok, Type *ty, bool is_local);
Type *find_defined_type(Token *tok);
Type *find_tagged_type(Token *tok);
void set_struct_member(Type *ty);

// scope.c
void new_scope();
void exit_scope();
void start_loop();
void end_loop();
void start_switch();
void end_switch();
int get_unique_num();

// util.c
void error(char *fmt);
void error1(char *fmt, char *v1);
void error2(char *fmt, char *v1, char *v2);
void error_at(char *loc, char *msg);
char *insert_str(char *src, int pos, char *target);
char *join_str(char *pre, char *post);
char *read_file(char *path);
char *extract_filename(char *path);
char *extract_dir(char *path);
char *remove_extension(char *filename);
#endif

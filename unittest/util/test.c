#include "../../util.c"

int testJoinStr()
{
    char *s1 = "hogehoge";
    char *s2 = "fugafuga";
    char *s;
    s = join_str(s1, s2);
    printf("%s\n", s);
    s = insert_str(s1, 0, s2);
    printf("%s\n", s);
    s = insert_str(s1, 1, s2);
    printf("%s\n", s);
    s = insert_str(s1, 2, s2);
    printf("%s\n", s);
    s = insert_str(s1, strlen(s1), s2);
    printf("%s\n", s);
    return 0;
}

int testExtractDir()
{
    char *s1 = "hogehoge/hoge";
    char *s2 = "fugafuga/hogehoge/fuga.c";
    char *s3 = "unittest/preprocessing_tokenize/include/10.c";
    char *s4 = "hoge.c";
    char *s5 = "";
    char *s;
    s = extract_dir(s1);
    printf("%s\n", s);
    s = extract_dir(s2);
    printf("%s\n", s);
    s = extract_dir(s3);
    printf("%s\n", s);
    s = extract_dir(s4);
    printf("%s\n", s);
    s = extract_dir(s5);
    printf("%s\n", s);
    return 0;
}

int testExtractFilename()
{
    char *s1 = "hogehoge/hoge";
    char *s2 = "fugafuga/hogehoge/fuga.c";
    char *s3 = "unittest/preprocessing_tokenize/include/10.c";
    char *s4 = "hoge.c";
    char *s5 = "";
    char *s;
    s = extract_filename(s1);
    printf("%s\n", s);
    s = extract_filename(s2);
    printf("%s\n", s);
    s = extract_filename(s3);
    printf("%s\n", s);
    s = extract_filename(s4);
    printf("%s\n", s);
    s = extract_filename(s5);
    printf("%s\n", s);
    return 0;
}

int testRemoveExtension()
{
    char *s1 = "hoge.c";
    char *s2 = "fuga.txt";
    char *s3 = "hogehoge.html.erb";
    char *s4 = "header.h";
    char *s5 = "";
    char *s;
    s = remove_extension(s1);
    printf("%s\n", s);
    s = remove_extension(s2);
    printf("%s\n", s);
    s = remove_extension(s3);
    printf("%s\n", s);
    s = remove_extension(s4);
    printf("%s\n", s);
    s = remove_extension(s5);
    printf("%s\n", s);
    return 0;
}

int main()
{
    testJoinStr();
    testExtractDir();
    testExtractFilename();
    testRemoveExtension();
}

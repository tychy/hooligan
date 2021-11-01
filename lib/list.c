#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 100
typedef struct List List;

struct List
{
    void **elm;
    int size;
    int cap;
};

List *new_list(int cap)
{
    List *li = calloc(1, sizeof(List));
    if (cap == 0)
    {
        cap = DEFAULT_CAPACITY;
    }
    li->cap = cap;
    li->elm = calloc(cap, sizeof(void *));
    li->size = 0;
    return li;
}

List *append(List *li, void *val)
{
    if (!li)
    {
        li = new_list(DEFAULT_CAPACITY);
        li->elm[0] = val;
        li->size = 1;
        return li;
    }
    if (li->size == li->cap - 1)
    {
        List *prev_li = li;
        li = new_list(2 * li->cap);
        memcpy(li->elm, prev_li->elm, prev_li->size * sizeof(void *));
        li->size = prev_li->size;
    }
    li->elm[li->size++] = val;
    return li;
}

static void assert_int(int expected, int actual)
{
    if (expected == actual)
    {
        return;
    }
    fprintf(stderr, "expected %d, but got %d\n", expected, actual);
    exit(1);
}

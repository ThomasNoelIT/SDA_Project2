#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "BST.h"
#include "BST2d.h"

/* ========================================================================= *
 * main in testTree
 * ========================================================================= */
int compare_doubles(void* a, void* b);

int compare_doubles(void* a, void* b)
{
    double* a_ = (double*)a;
    double* b_ = (double*)b;
    return (*a_ > *b_) - (*a_ < *b_);
}
 
int main(int argc, char const *argv[])
{
    //BST2d *tree = bst2dNew();
    BST *tree = bstNew(compare_doubles);

    int *key1, *value1;
    int *key2, *value2;
    int *key3, *value3;
    int *key4, *value4;
    int *key5, *value5;
    int *key6, *value6;
    int *key7, *value7;

    int k1 = 5;
    int v1 = 10;
    int k2 = 7;
    int v2 = 11;
    int k3 = 3;
    int v3 = 12;
    int k4 = 1;
    int v4 = 13;
    int k5 = 4;
    int v5 = 14;
    int k6 = 8;
    int v6 = 15;
    int k7 = 9;
    int v7 = 16;

    key1 = &k1;
    value1 = &v1;
    key2 = &k2;
    value2 = &v2;
    key3 = &k3;
    value3 = &v3;
    key4 = &k4;
    value4 = &v4;
    key5 = &k5;
    value5 = &v5;
    key6 = &k6;
    value6 = &v6;
    key7 = &k7;
    value7 = &v7;

    // bool a = bst2dInsert(tree, (void *) key1, (void *) value1);
    // bool b = bst2dInsert(tree, (void *) key2, (void *) value2);
    // bool c = bst2dInsert(tree, (void *) key3, (void *) value3);
    // bool d = bst2dInsert(tree, (void *) key4, (void *) value4);
    // bool e = bst2dInsert(tree, (void *) key5, (void *) value5);
    // bool f = bst2dInsert(tree, (void *) key6, (void *) value6);
    // bool g = bst2dInsert(tree, (void *) key7, (void *) value7);

    bstInsert(tree, (void *) key1, (void *) value1);
    bstInsert(tree, (void *) key2, (void *) value2);
    bstInsert(tree, (void *) key3, (void *) value3);
    bstInsert(tree, (void *) key4, (void *) value4);
    bstInsert(tree, (void *) key5, (void *) value5);
    bstInsert(tree, (void *) key6, (void *) value6);
    bstInsert(tree, (void *) key7, (void *) value7);

    //double lg = bst2dAverageNodeDepth(tree);
    double lg = bstAverageNodeDepth(tree);

    printf("the average length : %f\n", lg);

    //bst2dFree(tree, false, false);
    bstFree(tree, false, false);

    return 0;
}


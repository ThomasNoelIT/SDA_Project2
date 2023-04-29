/* ========================================================================= *
 * BST definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "List.h"

/* Opaque Structure */

typedef struct BNode_t BNode;

struct BNode_t
{
    BNode *parent;
    BNode *left;
    BNode *right;
    void *key;
    void *value;
};

struct BST_t
{
    BNode *root;
    size_t size;
    int (*compfn)(void *, void *);
};

/* Prototypes of static functions */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);
double bstAverageNodeDepthRecursive(BST *bst, BNode *node, int depth, int *count);
void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l);

/* Function definitions */

BNode *bnNew(void *key, void *value)
{
    BNode *n = malloc(sizeof(BNode));
    if (n == NULL)
    {
        printf("bnNew: allocation error\n");
        return NULL;
    }
    n->left = NULL;
    n->right = NULL;
    n->key = key;
    n->value = value;
    return n;
}

BST *bstNew(int comparison_fn_t(void *, void *))
{
    BST *bst = malloc(sizeof(BST));
    if (bst == NULL)
    {
        printf("bestNew: allocation error");
        return NULL;
    }
    bst->root = NULL;
    bst->size = 0;
    bst->compfn = comparison_fn_t;
    return bst;
}

void bstFreeRec(BNode *n, bool freeKey, bool freeValue)
{
    if (n == NULL)
        return;
    bstFreeRec(n->left, freeKey, freeValue);
    bstFreeRec(n->right, freeKey, freeValue);
    if (freeKey)
        free(n->key);
    if (freeValue)
        free(n->value);
    free(n);
}

void bstFree(BST *bst, bool freeKey, bool freeValue)
{
    bstFreeRec(bst->root, freeKey, freeValue);
    //free();
    free(bst);
}

size_t bstSize(BST *bst)
{
    return bst->size;
}

bool bstInsert(BST *bst, void *key, void *value)
{
    if (bst->root == NULL)
    {
        bst->root = bnNew(key, value);
        if (bst->root == NULL)
        {
            return false;
        }
        bst->size++;
        return true;
    }
    BNode *prev = NULL;
    BNode *n = bst->root;
    //printf("30\n");
    while (n != NULL)
    {
        prev = n;
        // printf("31\n");
        // printf("key = %p\n", key);
        // printf("n->key = %p\n", n->key);
        // printf("32\n");
        int cmp = bst->compfn(key, n->key);
        //printf("33\n");
        if (cmp <= 0)
        {
            n = n->left;
        }
        else
        {
            n = n->right;
        }
        //printf("34\n");
    }
    //printf("35\n");
    BNode *new = bnNew(key, value);
    if (new == NULL)
    {
        return false;
    }
    if (bst->compfn(key, prev->key) <= 0)
    {
        prev->left = new;
    }
    else
    {
        prev->right = new;
    }
    bst->size++;
    //printf("36\n");
    return true;
}

//------------------------------------------------------------------------- */

void *bstSearch(BST *bst, void *key)
{
    BNode *n = bst->root;
    while (n != NULL)
    {
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0)
        {
            n = n->left;
        }
        else if (cmp > 0)
        {
            n = n->right;
        }
        else
        {
            //printf("bstSearch: key found : %p\n", n->value);
            return n->value;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------- */
double bstAverageNodeDepthRecursive(BST *bst, BNode *node, int depth, int *count) {
    if (node == NULL) {
        return 0.0;
    }

    double leftSum = bstAverageNodeDepthRecursive(bst, node->left, depth+1, count);
    double rightSum = bstAverageNodeDepthRecursive(bst, node->right, depth+1, count);
    *count += 1;

    return (leftSum + rightSum + depth);
}

double bstAverageNodeDepth(BST *bst) {
    if (bst->root == NULL) {
        return 0.0;
    }

    int count = 0;
    double sum = bstAverageNodeDepthRecursive(bst, bst->root, 0, &count);

    return sum / count;
}
// liste des valeurs des clés mais attention à reprendre la position aussi => on peut stocker la position dans une nouvelle structure dans bst.c où on associe la valeur de la position
// une case pour le point pour filtrer les valeurs qui ne sont pas dans la boule et la valeur pour output (coordonnées)

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l)
{
    if (node == NULL){
        return;
    }

    int cmpMin = compare(keyMin, node->key);
    int cmpMax = compare(keyMax, node->key);
    bool sort;

    // Recursively search left subtree if keyMin is less than node's key
    if (cmpMin < 0){
        bstRangeSearchRec(node->left, keyMin, keyMax, compare, l);
    }

    // Add node's value to the list if node's key is within the specified range
    if (cmpMin <= 0 && cmpMax >= 0){
        sort = listInsertLast(l, node->key);
        if(sort == false)
            printf("bstRangeSearchRec: listInsertLast error\n");
    }

    // Recursively search right subtree if keyMax is greater than node's key
    if (cmpMax > 0){
        bstRangeSearchRec(node->right, keyMin, keyMax, compare, l);
    }
}

List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax)
{
    List *l = listNew();
    if (l == NULL)
        return NULL;

    bstRangeSearchRec(bst->root, keyMin, keyMax, bst->compfn, l);

    return l;
}




// List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax){
//     List *l = listNew();
//     if (l == NULL)
//         return NULL;

//     BNode *n = bst->root;
//     printf("1\n");
//     while (n != NULL)
//     {
//         printf("2\n");
//         int cmpMin = bst->compfn(keyMin, n->key);
//         int cmpMax = bst->compfn(keyMax, n->key);
//         bool sort;

//         if (n == NULL){
//             n = n->parent;
//         }

//         printf("3\n");
//         // Recursively search left subtree if keyMin is less than node's key
//         if (cmpMin < 0){
//             printf("10\n");
//             n = n->left;
//         }

//         printf("4\n");
//         // Add node's value to the list if node's key is within the specified range
//         if (cmpMin <= 0 && cmpMax >= 0){
//             printf("11\n");
//             sort = listInsertLast(l, n->key);
//             if(sort == false)
//                 printf("bstRangeSearchRec: listInsertLast error\n");
//             n = n->right;
//         }

//         printf("5\n");
//         // Recursively search right subtree if keyMax is greater than node's key
//         if (cmpMax > 0){
//             printf("12\n");
//             n = n->right;
//         }
//         printf("6\n");
//     }
//     printf("7\n");

//     return l;
// }



// List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax){
//     BNode *node = bst->root;

//     List *l = listNew();
//     if (l == NULL)
//         return NULL;

//     List *l2 = listNew();
//     if (l2 == NULL)
//         return NULL;

//     //printf("1\n");
//     //int i = 0;

//     while (node != NULL) {
//         // printf("i = %d\n", i);

//         //printf("2\n");
//         int cmpMin = bst->compfn(keyMin, node->key);
//         int cmpMax = bst->compfn(keyMax, node->key);
//         //printf("3\n");
//         if (node == NULL){
//             return l;
//         }
//         //printf("4\n");
//         if (cmpMin < 0) {
//             //printf("8\n");
//             node = node->left;
//         } else if (cmpMin <= 0 && cmpMax >= 0) {
//             //printf("9\n");
//             listInsertLast(l, node->key);
//             //printf("10\n");
//             l2 = bstRangeSearch(bst, keyMin, keyMax);
//             for(size_t i = 0; i < listSize(l2); i++){
//                 listInsertLast(l, l2->head->value);
//                 l2->head = l2->head->next;
//             }
//             //printf("11\n");
//             node = node->right;
//         } else {
//             //printf("12\n");
//             node = node->right;
//         }
//         // i++;
//         //printf("5\n");
//     }
//     //printf("6\n");

//     return l;
// }


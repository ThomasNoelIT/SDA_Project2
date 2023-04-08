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

void bstFree(BST *bst, bool freeKey, bool freeValue)
{
    bstFreeRec(bst->root, freeKey, freeValue);
    free(bst);
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
    while (n != NULL)
    {
        prev = n;
        int cmp = bst->compfn(key, n->key);
        if (cmp <= 0)
        {
            n = n->left;
        }
        else if (cmp > 0)
        {
            n = n->right;
        }
    }
    BNode *new = bnNew(key, value);
    if (new == NULL)
    {
        return false;
    }
    new->parent = prev;
    if (bst->compfn(key, prev->key) <= 0)
    {
        prev->left = new;
    }
    else
    {
        prev->right = new;
    }
    bst->size++;
    return true;
}

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
            return n->value;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
// A compléter

/*double bstAverageNodeDepth(BST *bst)
{
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
double bstAverageNodeDepth(BST *bst)
{
    if (bst->root == NULL)
    {
        return 0.0;
    }
    double sumDepth = 0.0;
    size_t countNodes = 0;
    List *q = listNew();
    listPushBack(q, bst->root);
    while (!listIsEmpty(q))
    {
        BNode *n = (BNode *)listFront(q);
        listPopFront(q);
        sumDepth += bstDepth(bst, n);
        countNodes++;
        if (n->left != NULL)
        {
            listPushBack(q, n->left);
        }
        if (n->right != NULL)
        {
            listPushBack(q, n->right);
        }
    }
    listFree(q, false);
    return sumDepth / countNodes;
}

static size_t bstDepth(BST *bst, BNode *n)
{
    size_t depth = 0;
    while (n != bst->root)
    {
        n = n->parent;
        depth++;
    }
    return depth;
}

/*double bstAverageNodeDepth(BST *bst)
{
    if (bst->root == NULL) {
        return 0.0;
    }

    int totalDepth = 0;
    int nodeCount = 0;
    List *stack = listNew();

    BNode *current = bst->root;

    while (current != NULL || !listIsEmpty(stack)) {
        if (current != NULL) {
            listPushFront(stack, current);
            current = current->left;
        } else {
            current = listPopFront(stack);
            totalDepth += bstNodeDepth(bst, current);
            nodeCount++;
            current = current->right;
        }
    }

    listFree(stack);

    return (double) totalDepth / nodeCount;
}

int bstNodeDepth(BST *bst, BNode *node)
{
    int depth = 0;
    while (node != NULL && node != bst->root) {
        node = node->parent;
        depth++;
    }
    return depth;
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*List *bstRangeSearch(BST *bst, void *keymin, void *keymax)
{
}*/
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax)
{
    List *l = listNew();
    if (l == NULL)
    {
        return NULL;
    }
    BNode *n = bst->root;
    while (n != NULL)
    {
        int cmpMin = bst->compfn(keyMin, n->key);
        int cmpMax = bst->compfn(keyMax, n->key);
        if (cmpMin <= 0 && cmpMax >= 0)
        {
            listPushBack(l, n->value);
        }
        if (cmpMin <= 0)
        {
            n = n->left;
        }
        else
        {
            n = n->right;
        }
    }
    return l;
}

    
/*List *bstRangeSearch(BST *bst, void *keymin, void *keymax){
    List *list = listNew();
    if (list == NULL)
    {
        printf("bstRangeSearch: allocation error\n");
        return NULL;
    }
    BNode *n = bst->root;
    while (n != NULL)
    {
        if (bst->compfn(n->key, keymin) < 0)
        {
            n = n->right;
        }
        else if (bst->compfn(n->key, keymax) > 0)
        {
            n = n->left;
        }
        else
        {
            listInsertAtEnd(list, n);
            BNode *pred = bstPredecessor(bst, n);
            while (pred != NULL && bst->compfn(pred->key, keymin) >= 0)
            {
                listInsertAtEnd(list, pred);
                pred = bstPredecessor(bst, pred);
            }
            BNode *succ = bstSuccessor(bst, n);
            while (succ != NULL && bst->compfn(succ->key, keymax) <= 0)
            {
                listInsertAtEnd(list, succ);
                succ = bstSuccessor(bst, succ);
            }
            return list;
        }
    }
    return list;
}*/


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


/* *********************************************************************************************************************
 *
 */
/*
struct BST {
    struct BSTNode *root;
    int (*compare)(const void *, const void *);
};
struct BSTNode {
    void *key;
    void *value;
    //int (*compare)(const void *, const void *);
    struct BSTNode *left;
    struct BSTNode *right;
};
 */
/* *********************************************************************************************************************
 *
 */


/* Prototypes of static functions */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);
void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l);
size_t bstDepth(BST *bst, BNode *node);


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

/* ------------------------------------------------------------------------- *
 * Returns the value associated to that key, if it belongs to the bst.
 * If duplicate copies of that key belongs to the bst, any one of the values
 * associated to that key is returned.
 *
 *
 * PARAMETERS
 * bst          A valid pointer to a BST object
 * key          The key to look for
 *
 * RETURN
 * res          One of the value corresponding to that key. Or NULL if the key
 *              is not present in the BST
 * ------------------------------------------------------------------------- */

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

/* *********************************************************************************************************************
************************************************************************************************************************
********************************************************************************************************************* */
size_t bstDepth(BST *bst, BNode *node)
{
    if (node == NULL) {
        return 0;
    }

    size_t depth = 0;
    BNode *current = bst->root;
    while (current != NULL && current != node) {
        if (node->value < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }

    return depth;
}

double bstAverageNodeDepth(BST *bst)
{
    printf("20\n");
    if (bst->root == NULL) {
        return 0.0;
    }

    double sumDepth = 0.0;
    size_t countNodes = 0;
    //printf("21\n");
    List *q = listNew();
    //printf("22\n");
    listInsertLast(q, bst->root);
    //printf("23\n");

    while (listSize(q) > 0) {
        //printf("29\n");
        if(q->head == NULL) {
            break;
        }
        //printf("30\n");
        //printf("value = %p\n", q->head->value);
        BNode *n = (BNode *)q->head->value;
        
        //printf("31\n");
        sumDepth += bstDepth(bst, n);
        countNodes++;
        //printf("32\n");
        if (n->left != NULL) {
            listInsertLast(q, n->left);
        }
        //printf("33\n");
        if (n->right != NULL) {
            listInsertLast(q, n->right);
        }
        //printf("34\n");

        // Remove the front element by updating the head of the list
        LNode *oldHead = q->head;
        //printf("35\n");
        q->head = q->head->next;
        //printf("36\n");
        free(oldHead);
        //printf("37\n");
    }
    //printf("24\n");

    listFree(q, false); // Pass false to not free the content of the list nodes
    //printf("25\n");
    return sumDepth / countNodes;
}


// liste des valeurs des clés mais attention à reprendre la position aussi => on peut stocker la position dans une nouvelle structure dans bst.c où on associe la valeur de la position
// une case pour le point pour filtrer les valeurs qui ne sont pas dans la boule et la valeur pour output (coordonnées)


/* *********************************************************************************************************************
 * *********************************************************************************************************************
********************************************************************************************************************* */

void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l)
{
    if (node == NULL)
        return;

    int cmpMin = compare(keyMin, node->key);
    int cmpMax = compare(keyMax, node->key);

    // Recursively search left subtree if keyMin is less than node's key
    if (cmpMin < 0)
        bstRangeSearchRec(node->left, keyMin, keyMax, compare, l);

    // Add node's value to the list if node's key is within the specified range
    if (cmpMin <= 0 && cmpMax >= 0)
        listInsertLast(l, node->value);

    // Recursively search right subtree if keyMax is greater than node's key
    if (cmpMax > 0)
        bstRangeSearchRec(node->right, keyMin, keyMax, compare, l);
}

List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax)
{
    List *l = listNew();
    if (l == NULL)
        return NULL;

    bstRangeSearchRec(bst->root, keyMin, keyMax, bst->compfn, l);

    return l;
}

// je sais pas si vraiment utile car on peut peut-être remplacer par compfn

/*
int compare(const void *a, const void *b)
{
    int *keyA = (int *)a;
    int *keyB = (int *)b;

    // Compare keys for sorting in increasing order
    if (*keyA < *keyB) {
        return -1;
    } else if (*keyA > *keyB) {
        return 1;
    } else {
        return 0;
    }
}*/

/* ========================================================================= *
 * BST definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "BST.h"
#include "List.h"

//--------------------------------------------------------------------------------------------------------------------------------
/* Opaque Structure */
//definition of the node of the binary search tree
typedef struct BNode_t BNode;

struct BNode_t{
    BNode *parent;
    BNode *left;
    BNode *right;
    void *key;
    void *value;
};

//definition of the binary search tree
struct BST_t{
    BNode *root;
    size_t size;
    int (*compfn)(void *, void *);
};

//--------------------------------------------------------------------------------------------------------------------------------
/* Prototypes of static functions */

static void bstFreeRec(BNode *n, bool freeKey, bool freeValue);
static BNode *bnNew(void *key, void *value);
void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l);

//--------------------------------------------------------------------------------------------------------------------------------
// Create a new BNode
BNode *bnNew(void *key, void *value){
    if(key == NULL){
        return NULL;
    }
    if(value == NULL){
        return NULL;
    }

    BNode *n = malloc(sizeof(BNode));
    if (n == NULL){
        //printf("bnNew: allocation error\n");
        return NULL;
    }

    n->left = NULL;
    n->right = NULL;
    n->key = key;
    n->value = value;
    return n;
}

//create a new BST
BST *bstNew(int comparison_fn_t(void *, void *)){

    if(comparison_fn_t == NULL){
        return NULL;
    }

    BST *bst = malloc(sizeof(BST));
    if (bst == NULL){
        //printf("bestNew: allocation error");
        return NULL;
    }

    bst->root = NULL;
    bst->size = 0;
    bst->compfn = comparison_fn_t;
    return bst;
}

//--------------------------------------------------------------------------------------------------------------------------------
// Free a BST recursively
void bstFreeRec(BNode *n, bool freeKey, bool freeValue){
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
// Free a BST
void bstFree(BST *bst, bool freeKey, bool freeValue){
    bstFreeRec(bst->root, freeKey, freeValue);
    free(bst);
}
//--------------------------------------------------------------------------------------------------------------------------------
// Return the size of a BST
size_t bstSize(BST *bst){
    if (bst == NULL){
        return 0;
    }
    return bst->size;
}

//--------------------------------------------------------------------------------------------------------------------------------
// Insert a new node in a BST
bool bstInsert(BST *bst, void *key, void *value){
    if (bst == NULL){
        return false;
    }
    if (key == NULL){
        return false;
    }
    if (value == NULL){
        return false;
    }

    if (bst->root == NULL){
        bst->root = bnNew(key, value);
        if (bst->root == NULL){
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
        if (cmp <= 0){
            n = n->left;
        }
        else if (cmp > 0){
            n = n->right;
        }
    }
    BNode *new = bnNew(key, value);
    if (new == NULL){
        return false;
    }
    new->parent = prev;
    if (bst->compfn(key, prev->key) <= 0){
        prev->left = new;
    }
    else{
        prev->right = new;
    }
    bst->size++;
    return true;
}
//--------------------------------------------------------------------------------------------------------------------------------
// Search a node in a BST
void *bstSearch(BST *bst, void *key){
    if (bst == NULL){
        return NULL;
    }
    if (key == NULL){
        return NULL;
    }

    BNode *n = bst->root;
    while (n != NULL){
        int cmp = bst->compfn(key, n->key);
        if (cmp < 0){
            n = n->left;
        }
        else if (cmp > 0){
            n = n->right;
        }
        else{
            return n->value;
        }
    }
    return NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------
//compute the range search recursively
void bstRangeSearchRec(BNode *node, void *keyMin, void *keyMax, int (*compare)(void *, void *), List *l){
    if (node == NULL){
        return;
    }
    if(keyMin == NULL){
        return;
    }
    if(keyMax == NULL){
        return;
    }
    if(compare == NULL){
        return;
    }
    if(l == NULL){
        return;
    }

    int cmpMin = compare(keyMin, node->key);
    int cmpMax = compare(keyMax, node->key);
    bool sort;

    if (cmpMin < 0){
        bstRangeSearchRec(node->left, keyMin, keyMax, compare, l);
    }

    if (cmpMin <= 0 && cmpMax >= 0){
        sort = listInsertLast(l, node->key);
        if(sort == false){
            //printf("bstRangeSearchRec: listInsertLast error\n");
            return;
        }
    }
    if (cmpMax > 0){
        bstRangeSearchRec(node->right, keyMin, keyMax, compare, l);
    }
}

//compute the range search
List *bstRangeSearch(BST *bst, void *keyMin, void *keyMax){
    if (bst == NULL){
        return NULL;
    }
    if (keyMin == NULL){
        return NULL;
    }
    if (keyMax == NULL){
        return NULL;
    }
    
    List *l = listNew();
    if (l == NULL)
        return NULL;

    bstRangeSearchRec(bst->root, keyMin, keyMax, bst->compfn, l);

    return l;
}
//--------------------------------------------------------------------------------------------------------------------------------
//compute the node depth recursively
static void bstAverageNodeDepthRec(BST *bst, BNode *node, int currNode_depth, int* sum_depth){
    if (node->right != NULL){
        *sum_depth += currNode_depth + 1;
        bstAverageNodeDepthRec(bst, node->right, currNode_depth + 1, sum_depth);
    }

    if (node->left != NULL){
       *sum_depth += currNode_depth + 1;
       bstAverageNodeDepthRec(bst, node->left, currNode_depth + 1, sum_depth);
    }
}

//compute the average node depth
double bstAverageNodeDepth(BST *bst){
    if (bst == NULL){
        return -1;
    }
    if (bst->root == NULL){
        return -1;
    }

    int *sum_depth = malloc(sizeof(int));
    *sum_depth = 0; 

    bstAverageNodeDepthRec(bst, bst->root, 0, sum_depth);

    double total = *sum_depth;
    free(sum_depth);

    double average_depth = total/(bst->size);

    return average_depth;

}
//--------------------------------------------------------------------------------------------------------------------------------


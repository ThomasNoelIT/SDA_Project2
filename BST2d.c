/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdbool.h>
#include <stdlib.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"

// A compléter

//----------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct BST2dNode_t BST2dNode;

struct BST2dNode_t {
    Point *p;
    BST2dNode *left;
    BST2dNode *right;
    bool vertical;
};

struct BST2d_t {
    BST2dNode *root;
    size_t size;
};
//----------------------------------------------------------------------------------------------------------------------------------------------------

static BST2dNode *bst2dNodeNew(Point *p, bool vertical) {
    BST2dNode *n = malloc(sizeof(BST2dNode));
    if (n == NULL) {
        printf("bst2dNodeNew: allocation error\n");
        return NULL;
    }
    n->p = p;
    n->left = NULL;
    n->right = NULL;
    n->vertical = vertical;
    return n;
}

static void bst2dNodeFree(BST2dNode *n) {
    if (n == NULL) {
        return;
    }
    bst2dNodeFree(n->left);
    bst2dNodeFree(n->right);
    pointFree(n->p);
    free(n);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

BST2d *bst2dNew(void) {
    BST2d *bst = malloc(sizeof(BST2d));
    if (bst == NULL) {
        printf("bst2dNew: allocation error\n");
        return NULL;
    }
    bst->root = NULL;
    bst->size = 0;
    return bst;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void bst2dFree(BST2d *bst) {
    bst2dNodeFree(bst->root);
    free(bst);
}

size_t bst2dSize(BST2d *bst) {
    return bst->size;
}

/*static BST2dNode *bst2dInsertRec(BST2dNode *n, Point *p, bool vertical) {
    /*if (n == NULL) {
        return bst2dNodeNew(p, vertical);
    }*/
    if (pointEquals(n->p, p)) {
        return n;
    }
    int cmp = vertical ? pointCompareByX(p, n->p) : pointCompareByY(p, n->p);
    if (cmp < 0) {
        n->left = bst2dInsertRec(n->left, p, !vertical);
    } else {
        n->right = bst2dInsertRec(n->right, p, !vertical);
    }
    return n;
}*/

void bst2dInsert(BST2d *bst, Point *p) {
    bst->root = bst2dInsertRec(bst->root, p, true);
    bst->size++;
}

// static void bst2dRangeSearchRec(BST2dNode *n, Rect rect, List *list) attention !!!

    } else {
        if (rect.bottom < n->p->y) {
            bst2dRangeSearchRec(n->left, rect, list);
        }
        if (rect.top >= n->p->y) {
            bst2dRangeSearchRec(n->right, rect

                                
//----------------------------------------------------------------------------------------------------------------------------------------------------

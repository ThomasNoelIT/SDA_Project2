/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"

//--------------------------------------------------------------------------------------------------------------------------------

// BST2dNode structure definition
typedef struct BST2dNode_t BST2dNode;

struct BST2dNode_t {
    Point *p;
    BST2dNode *left;
    BST2dNode *right;
    bool vertical;
    void* value;
};

// BST2d structure definition
struct BST2d_t {
    BST2dNode *root;
    size_t size;
};

//--------------------------------------------------------------------------------------------------------------------------------

/* Prototypes of static functions */
static void bst2dAverageNodeDepthRec(BST2dNode *node, int* total_depth, int node_depth);
static void bst2dBallSearchRec(BST2d *bst2d,Point *q,double r, BST2dNode *node,List *l);

//--------------------------------------------------------------------------------------------------------------------------------
// Create a new BST2dNode
static BST2dNode *BST2dNode_new(Point *point, bool vertical,void* value) {
    if(point == NULL){
        return NULL;
    }
    if(value == NULL){
        return NULL;
    }

    BST2dNode *node = malloc(sizeof(BST2dNode));
    if (node == NULL) {
        return NULL;
    }

    node->p = point;
    node->vertical = vertical;
    node->left = NULL;
    node->right = NULL;
    node->value = value;
    return node;
}

// Create a new BST2d
BST2d *bst2dNew(void){
    BST2d *tree = malloc(sizeof(BST2d));
    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    return tree;
}
//--------------------------------------------------------------------------------------------------------------------------------
//free recursively the BST2d
static void BST2drec(BST2dNode *node, bool freeKey, bool freeValue ){
    if (node == NULL) {
        return;
    }
    BST2drec(node->left,freeKey,freeValue);
    BST2drec(node->right,freeKey,freeValue);
    if(freeValue){
        free(node->value);
    }
    if(freeKey){
        ptFree(node->p);
    }
    free(node);
    
}

// Free a BST2d
void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2drec(bst2d->root,freeKey,freeValue);
    free(bst2d);
}

//--------------------------------------------------------------------------------------------------------------------------------
//insert a node in the BST2d
static bool insertion(BST2dNode *node, Point *point, bool vertical,void* value) {
    if (node == NULL) {
        return false;
    }
    if(point == NULL){
        return false;
    }
    if(value == NULL){
        return false;
    }

    //if vertical is true, we compare the x of the point, else we compare the y
    if (vertical) {
        if (ptGetx(point) <= ptGetx(node->p)) {
            if (node->left == NULL) {
                node->left = BST2dNode_new(point, !vertical,value);
                return true;
            } else {
                return insertion(node->left, point, !vertical,value);
            }
        } else {
            if (node->right == NULL) {
                node->right = BST2dNode_new(point, !vertical,value);
                return true;
            } else {
                return insertion(node->right, point, !vertical,value);
            }
        }
    } else {
        if (ptGety(point) <= ptGety(node->p)) {
            if (node->left == NULL) {
                node->left = BST2dNode_new(point, !vertical,value);
                return true;
            } else {
                return insertion(node->left, point, !vertical,value);
            }
        } else {
            if (node->right == NULL) {
                node->right = BST2dNode_new(point, !vertical,value);
                return true;
            } else {
                return insertion(node->right, point, !vertical,value);
            }
        }
    }
}

// Insert a point in the BST2d
bool bst2dInsert(BST2d *b2d, Point *point, void *value) {
    if (b2d == NULL) {
        return false;
    }
    if(point == NULL){
        return false;
    }
    if(value == NULL){
        return false;
    }

    b2d->size++;
    if(b2d->root == NULL){
        b2d->root = BST2dNode_new(point, true,value);
        if(b2d->root == NULL){
            return false;
        }
        return true;
    }

    bool temp = insertion(b2d->root, point, true,value);
    return temp;
}

//--------------------------------------------------------------------------------------------------------------------------------
//return the bst2d size
size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

//--------------------------------------------------------------------------------------------------------------------------------
//search the node with the key point q
void *bst2dSearch(BST2d *b2d, Point *q){
    if (b2d == NULL) {
        return NULL;
    }
    if(q == NULL){
        return NULL;
    }

    BST2dNode *node = b2d->root;
    if(node == NULL){
        return NULL;
    }

    while (node != NULL) {
        if (ptCompare(node->p, q) == 0) {
            return node->value;
        }
        //if vertical is true, we compare the x of the point, else we compare the y
        if (node->vertical) {
            if (ptGetx(q) < ptGetx(node->p)) {
                node = node->left;
            } else {
                node = node->right;
            }
        } else {
            if (ptGety(q) < ptGety(node->p)) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
    }
    return NULL;
}

// ------------------------------------------------------------------------------------------------------
//parcours the tree recursively and insert the points in the list 
static void bst2dBallSearchRec(BST2d *bst2d,Point *q,double r, BST2dNode *node,List *l){
    if(bst2d == NULL){
        return;
    }
    if (node == NULL) {
        return;
    }
    if(q == NULL){
        return;
    }
    if(r < 0){
        return;
    }
    if(l == NULL){
        return;
    }
    bool temp;

    if (ptSqrDistance(node->p, q) <= r * r) {
        temp = listInsertLast(l, node->p);
        if (temp == false) {
            return;
        }
    }
    if (node->left != NULL) {
        bst2dBallSearchRec(bst2d,q,r,node->left,l);
    }
    if (node->right != NULL) {
        bst2dBallSearchRec(bst2d,q,r,node->right,l);
    }
}

//return the list of the points in a radius r of the center point q
List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    if (bst2d == NULL) {
        return NULL;
    }
    if(q == NULL){
        return NULL;
    }
    if(r < 0){
        return NULL;
    }

    List *l = listNew();
    if (l == NULL) {
        return NULL;
    }

    BST2dNode *node = bst2d->root;
    if (node == NULL) {
        return NULL;
    }

    bst2dBallSearchRec(bst2d,q,r,node,l);
    return l;
}

// -------------------------------------------------------------------------------------------------------
//parcours the tree recursively and count the number of points in the rectangle
static void bst2dAverageNodeDepthRec(BST2dNode *node, int* total_depth, int node_depth){
    if (node == NULL) {
        return;
    }
    if(total_depth == NULL){
        return;
    }
    if(node_depth < 0){
        return;
    }

    if (node->left != NULL){
       *total_depth += node_depth + 1;
       bst2dAverageNodeDepthRec(node->left, total_depth, node_depth + 1);
    }
    if (node->right != NULL){
        *total_depth += node_depth + 1;
        bst2dAverageNodeDepthRec(node->right, total_depth, node_depth + 1);
    }
}

//return the average depth of the nodes in the tree
double bst2dAverageNodeDepth(BST2d *bst2d){
    if (bst2d == NULL) {
        return -1;
    }
    if(bst2d->root == NULL){
        return -1;
    }

    int *total_depth = malloc(sizeof(int));
    if (total_depth == NULL) {
        return -1;
    }

    *total_depth = 0; 

    bst2dAverageNodeDepthRec(bst2d->root, total_depth, 0);

    double average_depth = (double)*total_depth/(bst2d->size);

    free(total_depth);

    return average_depth;
}

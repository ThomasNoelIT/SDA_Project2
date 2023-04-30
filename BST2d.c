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

typedef struct BST2dNode_t BST2dNode;
//void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* depth);

typedef struct Sum_t {
    size_t sum;
    size_t nb_nodes;
} Sum;

struct BST2dNode_t {
    Point *p;
    BST2dNode *left;
    BST2dNode *right;
    bool vertical;
    void* value;
};

struct BST2d_t {
    BST2dNode *root;
    size_t size;
};

size_t bst2dCountNodes(BST2dNode *node);

/* ========================================================================= *
 * BST2d interaction functions
 * ========================================================================= */

static BST2dNode *BST2dNode_new(Point *point, bool vertical,void* value) {
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

BST2d *bst2dNew(void){
    //printf("bst2dNew\n");
    BST2d *tree = malloc(sizeof(BST2d));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

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

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2drec(bst2d->root,freeKey,freeValue);
    free(bst2d);
}

static bool insertion(BST2dNode *node, Point *point, bool vertical,void* value) {
    //printf("insertion\n");
    if (node == NULL) {
        return false;
    }
    // if (ptGetx(point) == ptGetx(node->p) && ptGety(point) == ptGety(node->p)) {
    //     return false;
    // }
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

bool bst2dInsert(BST2d *b2d, Point *point, void *value) {
    if (b2d == NULL) {
        return false;
    }
    b2d->size++;
    if(b2d->root == NULL){
        b2d->root = BST2dNode_new(point, true,value);
        return true;
    }
    bool temp = insertion(b2d->root, point, true,value);
    return temp;
}

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}


void *bst2dSearch(BST2d *b2d, Point *q){
    BST2dNode *node = b2d->root;
    if(node == NULL){
        return NULL;
    }
    while (node != NULL) {
        if (ptCompare(node->p, q) == 0) {
            return node->value;
        }
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
static void bst2dBallSearchRecursive(BST2d *bst2d,Point *q,double r, BST2dNode *node,List *l);

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    // ajouter if supp pour intput
    List *l = listNew();
    if (l == NULL) {
        return NULL;
    }
    BST2dNode *node = bst2d->root;
    if (node == NULL) {
        return NULL;
    }
    bst2dBallSearchRecursive(bst2d,q,r,node,l);
    return l;
}

static void bst2dBallSearchRecursive(BST2d *bst2d,Point *q,double r, BST2dNode *node,List *l){
    if (node == NULL) {
        return;
    }
    if (ptSqrDistance(node->p, q) <= r * r) {
        listInsertLast(l, node->p);
    }
    if (node->left != NULL) {
        bst2dBallSearchRecursive(bst2d,q,r,node->left,l);
    }
    if (node->right != NULL) {
        bst2dBallSearchRecursive(bst2d,q,r,node->right,l);
    }
}

// -------------------------------------------------------------------------------------------------------
static void bst2dAverageNodeDepthRecursive(BST2dNode *node, int* total_depth, int node_depth);

// Cette fonction calcule la profondeur moyenne des nœuds dans un BST2D
static void bst2dAverageNodeDepthRecursive(BST2dNode *node, int* total_depth, int node_depth){
    if (node->left != NULL){
       *total_depth += node_depth + 1;
       bst2dAverageNodeDepthRecursive(node->left, total_depth, node_depth + 1);
    }
    if (node->right != NULL){
        *total_depth += node_depth + 1;
        bst2dAverageNodeDepthRecursive(node->right, total_depth, node_depth + 1);
    }
}

double bst2dAverageNodeDepth(BST2d *bst2d){
    int *total_depth = malloc(sizeof(int));
    *total_depth = 0; 

    bst2dAverageNodeDepthRecursive(bst2d->root, total_depth, 0);

    double average_depth = (double)*total_depth/(bst2d->size);

    free(total_depth);

    return average_depth;
}

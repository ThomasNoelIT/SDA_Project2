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

/* ========================================================================= *
 * BST2d interaction functions
 * ========================================================================= */

void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* depth);

//--------------------------------------------------------------------------------------------------------

static BST2dNode *BST2dNode_new(double x, double y, bool vertical,void* value) {
    BST2dNode *node = malloc(sizeof(BST2dNode));
    if (node == NULL) {
        return NULL;
    }
    node->p = ptNew(x, y);
    node->vertical = vertical;
    node->left = NULL;
    node->right = NULL;
    node->value = value;
    return node;
}

BST2d *bst2dNew(void){
    BST2d *tree = malloc(sizeof(BST2d));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

//--------------------------------------------------------------------------------------------------------

static void BST2dNode_free(BST2dNode *node, bool freeKey, bool freeValue, bool all) {
    if (node == NULL) {
        return;
    }
    if(freeValue){
        free(node->value);
    }
    if(freeKey){
        ptFree(node->p);
    }
    if (all) {
        BST2dNode_free(node->left,freeKey,freeValue,all);
        BST2dNode_free(node->right,freeKey,freeValue,all);
    }
    free(node);
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2dNode_free(bst2d->root,freeKey,freeValue,true);
    free(bst2d);
}

//--------------------------------------------------------------------------------------------------------

static bool insertion(BST2dNode *node, Point *point, bool vertical,void* value) {
    if (node == NULL) {
        return false;
    }
    if (ptGetx(point) == ptGetx(node->p) && ptGety(point) == ptGety(node->p)) {
        return false;
    }
    if (vertical) {
        if (ptGetx(point) < ptGetx(node->p)) {
            if (node->left == NULL) {
                node->left = BST2dNode_new(ptGetx(point), ptGety(point), !vertical,value);
                return true;
            } else {
                return insertion(node->left, point, !vertical,value);
            }
        } else {
            if (node->right == NULL) {
                node->right = BST2dNode_new(ptGetx(point), ptGety(point), !vertical,value);
                return true;
            } else {
                return insertion(node->right, point, !vertical,value);
            }
        }
    } else {
        if (ptGety(point) < ptGety(node->p)) {
            if (node->left == NULL) {
                node->left = BST2dNode_new(ptGetx(point), ptGety(point), !vertical,value);
                return true;
            } else {
                return insertion(node->left, point, !vertical,value);
            }
        } else {
            if (node->right == NULL) {
                node->right = BST2dNode_new(ptGetx(point), ptGety(point), !vertical,value);
                return true;
            } else {
                return insertion(node->right, point, !vertical,value);
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------------

bool bst2dInsert(BST2d *b2d, Point *point, void *value) {
    if (b2d == NULL) {
        return false;
    }
    b2d->size++;
    if(b2d->root == NULL){
        b2d->root = BST2dNode_new(ptGetx(point), ptGety(point), true,value);
        return true;
    }
    bool temp = insertion(b2d->root, point, true,value);
    return temp;
}

//--------------------------------------------------------------------------------------------------------

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

//--------------------------------------------------------------------------------------------------------

void *bst2dSearch(BST2d *b2d, Point *q){
    if(b2d == NULL){
        printf("Error in bst2dSearch : b2d is NULL.\n");
        return NULL;
    }
    BST2dNode *node = b2d->root;
    if(node == NULL){
        printf("Error in bst2dSearch : node is NULL.\n");
        return NULL;
    }
    while (node != NULL) {
        printf("Node actuel (%d, %d)\n", ptGetx(node->p), ptGetx(node->p));
        printf("Comparaison de (%d, %d)\n", ptGetx(q), ptGetx(node->p));
        printf("Comparaison de (%d, %d)\n", ptGety(q), ptGety(node->p));
        if (ptCompare(node->p, q) == 0) {
            printf("Aucune différence.\n");
            return node->value;
        } else if (node->vertical) {
            if (ptGetx(q) < ptGetx(node->p)) {
                printf("Comparaison selon x et va à gauche.\n");
                node = node->left;
            } else {
                printf("Comparaison selon x et va à droite.\n");
                node = node->right;
            }
        } else {
            if (ptGety(q) < ptGety(node->p)) {
                printf("Comparaison selon y et va à gauche.\n");
                node = node->left;
            } else {
                printf("Comparaison selon y et va à droite.\n");
                node = node->right;
            }
        }
    }
    printf("Sortie de boucle.\n");
    printf("Recherche de la position (%d, %d)\n", ptGetx(q), ptGety(q));
    return NULL;
}

//--------------------------------------------------------------------------------------------------------

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    List *l = listNew();
    if (l == NULL) {
        printf("Error in bst2dBallSearch : l is NULL.\n");
        return NULL;
    }
    BST2dNode *node = bst2d->root;
    while (node != NULL) {
        if (ptSqrDistance(node->p, q) <= r) {
            listInsertLast(l, node->p);
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
    return l;
}

void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* all_depth) {
    if (node == NULL) {
        return;
    }
    void *current_depth_temp = (void*) &current_depth;
    listInsertLast(all_depth, current_depth_temp);
    current_depth++;
    bst2dAverageNodeDepthRecursive(node->left, current_depth, all_depth);
    bst2dAverageNodeDepthRecursive(node->right, current_depth, all_depth);
}

double bst2dAverageNodeDepth(BST2d *bst2d) {
    if (bst2d == NULL){
        printf("Error in bst2dAverageNodeDepth : bst2d is NULL.\n");
        return 0.0;
    }
    if (bst2d->root == NULL) {
        printf("Error in bst2dAverageNodeDepth : root is NULL.\n");
        return 0.0;
    }
    
    int size_tree = bst2d->size;

    List *all_depth = listNew();
    if (all_depth == NULL){
        printf("Error in bst2dAverageNodeDepth : all_depth is NULL.\n");
        return 0.0;
    }
    
    bst2dAverageNodeDepthRecursive(bst2d->root, 0, all_depth);

    double average = 0.0;
    LNode *currDepth = all_depth->head;
    while(currDepth != NULL){
        int extractedValue = *(int*)(currDepth->value);
        average += (double)extractedValue;
        currDepth = currDepth->next;
    }

    average /= size_tree;

    listFree(all_depth,true);
    free(all_depth);
    return average;
}

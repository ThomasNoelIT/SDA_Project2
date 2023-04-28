
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
void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* depth);

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

// //print bst2d recursive
// void bst2dPrintRecursive(BST2dNode *node){
//     if (node == NULL) {
//         return;
//     }
//     printf("node = (%f,%f)\n",ptGetx(node->p),ptGety(node->p));
//     bst2dPrintRecursive(node->left);
//     bst2dPrintRecursive(node->right);
// }

// //print bst2d
// void bst2dPrint(BST2d *bst2d){
//     BST2dNode *node = bst2d->root;
//     printf("root = (%f,%f)\n",ptGetx(node->p),ptGety(node->p));
//     bst2dPrintRecursive(node->left);
//     bst2dPrintRecursive(node->right);
// }

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2dNode_free(bst2d->root,freeKey,freeValue,true);
    free(bst2d);
}

static bool insertion(BST2dNode *node, Point *point, bool vertical,void* value) {
    //printf("insertion\n");
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

bool bst2dInsert(BST2d *b2d, Point *point, void *value) {
    // if (b2d->root == NULL) {
    //     b2d->root = BST2dNode_new(ptGetx(point), ptGety(point), vertical);
    //     b2d->size++;
    //     return node;
    // }
    // if (vertical) {
    //     if (ptGetx(point) < ptGetx(node->p)) {
    //         node->left = BST2dNode_insert(node->left, p, !vertical);
    //     } else {
    //         node->right = BST2dNode_insert(node->right, p, !vertical);
    //     }
    // } else {
    //     if (ptGety(p) < ptGety(node->p)) {
    //         node->left = BST2dNode_insert(node->left, p, !vertical);
    //     } else {
    //         node->right = BST2dNode_insert(node->right, p, !vertical);
    //     }
    // }
    // return node;
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

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

// bool bst2dInsert(BST2d *b2d, Point *point, void *value){
//     if (b2d == NULL) {
//         return false;
//     }
//     b2d->root = BST2dNode_insert(b2d->root, point, value);
//     b2d->size++;
//     return true;
// }

/* ========================================================================= *
 * BST2d functions
 * ========================================================================= */
/* ------------------------------------------------------------------------- *
 * Returns the value associated to a position, if any. If several values are
 * associated to this position, any one of them is returned.
 *
 * PARAMETERS
 * bst          A valid pointer to a BST2d object
 * q            The position to look for
 *
 * RETURN
 * res          One of the value corresponding to that key. Or NULL if the key
 *              is not present in the BST2d
 * ------------------------------------------------------------------------- */

void *bst2dSearch(BST2d *b2d, Point *q){
    // printf("\n");
    // printf("0\n");
    //printf("b2d point : x = %f, y = %f",ptGetx(b2d->root->p),ptGety(b2d->root->p));
    // printf("1\n");
    BST2dNode *node = b2d->root;
    // printf("2\n");
    if(node == NULL){
        return NULL;
    }
    // printf("3\n");
    while (node != NULL) {
        //printf("je rentre dans la boucle\n");
        // printf("5\n");
        if (ptCompare(node->p, q) == 0) {
            // printf("\n");
            // printf("point : x = %f, y = %f\n",ptGetx(node->p),ptGety(node->p));
            return node->value;
        }
        //printf("6\n");
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
        //printf("7\n");
    }
    //printf("4\n");
    return NULL;
}

/* ------------------------------------------------------------------------- *
 * Finds the set of positions (x,y) in the provided BST2d that are included
 * in a ball of radius r and centered at the position q given as argument.
 * The function returns a list of the values associated to these positions
 * (in no particular order).
 *
 * PARAMETERS
 * bst2d          A valid pointer to a BST2d object
 * q              The center of the ball
 * r              The radius of the ball
 *
 * RETURN
 * l              A List containing the values in the given ball, or
 *                NULL in case of allocation error.
 *
 * NOTES
 * The List must be freed but not its content. If no elements are in the
 * ball, the function returns an empty list
 * ------------------------------------------------------------------------- */

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    List *l = listNew();
    if (l == NULL) {
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

/* ------------------------------------------------------------------------- *
 * Returns the average depth of the BST2d nodes. The depth of a node is the
 * number of edges that connect it to the root.
 *
 * PARAMETERS
 * bst2d          A valid pointer to a BST2d object
 *
 * RETURN
 * double         The average depth of the BST2d nodes
 * ------------------------------------------------------------------------- */
// Déclaration de la fonction bst2dCountNodes()
size_t bst2dCountNodes(BST2dNode *node);



// Fonction pour calculer le nombre de nœuds dans un BST2D
size_t bst2dCountNodes(BST2dNode *node) {
    if (node == NULL) {
        return 0;
    }

    // Utilisation d'une approche récursive pour compter les nœuds dans le sous-arbre gauche et le sous-arbre droit
    size_t leftCount = bst2dCountNodes(node->left);
    size_t rightCount = bst2dCountNodes(node->right);

    // Retourne le total des nœuds du sous-arbre gauche, du sous-arbre droit et du nœud courant
    return leftCount + rightCount + 1;
}

void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* all_depth) {
    if (node == NULL) {
        return;
    }
    void* current_depth_temp = (void*) &current_depth;
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
        printf("Error in bst2dAverageNodeDepth : size_tree is NULL.\n");
        return 0.0;
    }
    bst2dAverageNodeDepthRecursive(bst2d->root, 0, all_depth);

    double average = 0.0;
    //size_t nodeCount = bst2dCountNodes(bst2d->root);
    /*LNode *currDepth = all_depth->head;
    while(currDepth != NULL){
        double extractedValue = *(double*)(currDepth->value);
        average += extractedValue;
        currDepth = currDepth->next;
    }*/
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

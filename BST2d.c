/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */
/*
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"

typedef struct BST2dNode_t BST2dNode;
void bst2dAverageNodeDepthRecursive(BST2dNode *node, void* current_depth, List* depth);

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
*/
/* ========================================================================= *
 * BST2d interaction functions
 * ========================================================================= */
/*
static BST2dNode *BST2dNode_new(void) {
    BST2dNode *node = malloc(sizeof(BST2dNode));
    if (node == NULL) {
        return NULL;
    }
    node->p = ptNew(0, 0);
    node->left = NULL;
    node->right = NULL;
    return node;
}

BST2d *bst2dNew(void){
    BST2d *tree = malloc(sizeof(BST2d));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = BST2dNode_new();
    tree->size = 0;
    return tree;
}

static void BST2dNode_free(BST2dNode *node, bool freeKey, bool freeValue) {
    if (node == NULL) {
        return;
    }
    ptFree(node->p);
    free(node);
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2dNode_free(bst2d->root,freeKey,freeValue);
    free(bst2d);
}

static BST2dNode *BST2dNode_insert(BST2dNode *node, Point *p, bool vertical) {
    if (node == NULL) {
        node = BST2dNode_new();
        node->p = p;
        node->vertical = vertical;
        return node;
    }
    if (vertical) {
        if (ptGetx(p) < ptGetx(node->p)) {
            node->left = BST2dNode_insert(node->left, p, !vertical);
        } else {
            node->right = BST2dNode_insert(node->right, p, !vertical);
        }
    } else {
        if (ptGety(p) < ptGety(node->p)) {
            node->left = BST2dNode_insert(node->left, p, !vertical);
        } else {
            node->right = BST2dNode_insert(node->right, p, !vertical);
        }
    }
    return node;
}

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

bool bst2dInsert(BST2d *b2d, Point *point, void *value){
    if (b2d == NULL) {
        return false;
    }
    b2d->root = BST2dNode_insert(b2d->root, point, value);
    b2d->size++;
    return true;
}
*/
/* ========================================================================= *
 * BST2d functions
 * ========================================================================= */
/*
void *bst2dSearch(BST2d *b2d, Point *q){
    BST2dNode *node = b2d->root;
    while (node != NULL) {
        if (ptCompare(node->p, q) == 0) {
            return node->p;
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

double bst2dAverageNodeDepth(BST2d *bst2d) {
    double average = 0.0;
    int size_tree = bst2d->size;
    void* value;
    printf("10\n");
    List* depth = listNew();
    if (depth == NULL) {
        return 0.0;
    }
    printf("11\n");

    bst2dAverageNodeDepthRecursive(bst2d->root, 0, depth);

    printf("12\n");

    for (int i = 0; i < size_tree-1; i++) {
        printf("20\n");
        //printf("value = %d",*(int*)depth->head->value);
        //value = *(int*) depth->head->value;
        
        value = depth->head->value;
        printf("21\n");
        //printf("value = %p\n",value);
        printf("22\n");
        // int *test = value;

        average += (int)(intptr_t)value;
        printf("23\n");
    }
    printf("13\n");
    printf("average = %a\n",average);
    average /= size_tree;
    printf("average = %a\n",average);

    printf("14\n");

    listFree(depth,1);

    printf("15\n");

    free(depth);

    printf("16\n");


    return average;
}

void bst2dAverageNodeDepthRecursive(BST2dNode *node, void* current_depth, List* depth) {
    if (node == NULL) {
        return;
    }

    listInsertLast(depth, current_depth);

    bst2dAverageNodeDepthRecursive(node->left, current_depth+1, depth);
    bst2dAverageNodeDepthRecursive(node->right, current_depth+1, depth);
}
*/

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
void bst2dAverageNodeDepthRecursive(BST2dNode *node, void* current_depth, List* depth);

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

/* ========================================================================= *
 * BST2d interaction functions
 * ========================================================================= */

static BST2dNode *BST2dNode_new(void) {
    BST2dNode *node = malloc(sizeof(BST2dNode));
    if (node == NULL) {
        return NULL;
    }
    node->p = ptNew(0, 0);
    node->left = NULL;
    node->right = NULL;
    return node;
}

BST2d *bst2dNew(void){
    BST2d *tree = malloc(sizeof(BST2d));
    if (tree == NULL) {
        return NULL;
    }
    tree->root = BST2dNode_new();
    tree->size = 0;
    return tree;
}

static void BST2dNode_free(BST2dNode *node, bool freeKey, bool freeValue) {
    if (node == NULL) {
        return;
    }
    ptFree(node->p);
    free(node);
}

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2dNode_free(bst2d->root,freeKey,freeValue);
    free(bst2d);
}

static BST2dNode *BST2dNode_insert(BST2dNode *node, Point *p, bool vertical) {
    if (node == NULL) {
        node = BST2dNode_new();
        node->p = p;
        node->vertical = vertical;
        return node;
    }
    if (vertical) {
        if (ptGetx(p) < ptGetx(node->p)) {
            node->left = BST2dNode_insert(node->left, p, !vertical);
        } else {
            node->right = BST2dNode_insert(node->right, p, !vertical);
        }
    } else {
        if (ptGety(p) < ptGety(node->p)) {
            node->left = BST2dNode_insert(node->left, p, !vertical);
        } else {
            node->right = BST2dNode_insert(node->right, p, !vertical);
        }
    }
    return node;
}

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

bool bst2dInsert(BST2d *b2d, Point *point, void *value){
    if (b2d == NULL) {
        return false;
    }
    b2d->root = BST2dNode_insert(b2d->root, point, value);
    b2d->size++;
    return true;
}

/* ========================================================================= *
 * BST2d functions
 * ========================================================================= */

void *bst2dSearch(BST2d *b2d, Point *q){
    BST2dNode *node = b2d->root;
    while (node != NULL) {
        if (ptCompare(node->p, q) == 0) {
            return node->p;
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
    size_t nodeCount = bst2dCountNodes(bst2d->root);
    LNode *currDepth = all_depth->head;
    while(currDepth != NULL){
        double extractedValue = *(double*)(currDepth->value);
        average += extractedValue;
        currDepth = currDepth->next;
    }
    average /= size_tree;
    listFree(all_depth,1);
    free(all_depth);
    return average;
}

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

void bst2dAverageNodeDepthRecursive(BST2dNode *node, void* current_depth, List* all_depth) {
    if (node == NULL) {
        return;
    }
    listInsertLast(all_depth, current_depth);

    bst2dAverageNodeDepthRecursive(node->left, current_depth+1, all_depth);
    bst2dAverageNodeDepthRecursive(node->right, current_depth+1, all_depth);
}


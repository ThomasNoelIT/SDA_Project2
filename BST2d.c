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

//void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* depth);

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


typedef struct Sum_t {
    size_t sum;
    size_t nb_nodes;
} Sum;


size_t bst2dCountNodes(BST2dNode *node);
Sum bst2dAverageNodeDepthRecursive(BST2dNode*node);

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

//--------------------------------------------------------------------------------------------------------

void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    BST2drec(bst2d->root,freeKey,freeValue);
    free(bst2d);
}

//--------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------

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

//--------------------------------------------------------------------------------------------------------

size_t bst2dSize(BST2d *bst2d){
    return bst2d->size;
}

//--------------------------------------------------------------------------------------------------------

void *bst2dSearch(BST2d *b2d, Point *q){
    BST2dNode *node = b2d->root;
    // if(node == NULL){
    //     return NULL;
    // }
    while (node != NULL) {
        //printf("Node actuel (%f, %f)\n", ptGetx(node->p), ptGety(node->p));
        //printf("Adresse de la valeur trouvée : %p\n", (void *) node->value);
        if (ptCompare(node->p, q) == 0) {
            //printf("Adresse de la valeur trouvée : %p\n", (void *) node->value);
            //printf("Correspondance dont la valeur vaut : (%f)\n",node->value);
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
    //printf("Sortie de boucle.\n");
    return NULL;
}

//--------------------------------------------------------------------------------------------------------

List *bst2dBallSearch(BST2d *bst2d, Point *q, double r){
    List *l = listNew();
    if (l == NULL) {
        return NULL;
    }
    BST2dNode *node = bst2d->root;
    while (node != NULL) {
        if (ptSqrDistance(node->p, q) <= r * r) {
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

//--------------------------------------------------------------------------------------------------------

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

// void bst2dAverageNodeDepthRecursive(BST2dNode *node, int current_depth, List* all_depth) {
//     if (node == NULL) {
//         return;
//     }
//     // void* current_depth_temp = (void*) &current_depth;
//     // listInsertLast(all_depth, current_depth_temp);
//     // current_depth++;
//     // bst2dAverageNodeDepthRecursive(node->left, current_depth, all_depth);
//     // bst2dAverageNodeDepthRecursive(node->right, current_depth, all_depth);


// }

Sum bst2dAverageNodeDepthRecursive(BST2dNode*node){
    if (node == NULL){
        Sum sum = {0,0};
        return sum;
    }
    Sum left = bst2dAverageNodeDepthRecursive(node->left);
    Sum right = bst2dAverageNodeDepthRecursive(node->right);
    size_t nb_nodes = left.nb_nodes + right.nb_nodes + 1;
    size_t sum = left.sum + right.sum + nb_nodes - 1;
    Sum sum2 = {nb_nodes, sum};
    return sum2;
}


double bst2dAverageNodeDepth(BST2d *bst2d) {
    printf("bst2dAverageNodeDepth\n");
    if (bst2d == NULL){
        printf("Error in bst2dAverageNodeDepth : bst2d is NULL.\n");
        return 0.0;
    }
    if (bst2d->root == NULL) {
        printf("Error in bst2dAverageNodeDepth : root is NULL.\n");
        return 0.0;
    }
    //int size_tree = bst2d->size;

    //List *all_depth = listNew();
    // if (all_depth == NULL){
    //     printf("Error in bst2dAverageNodeDepth : size_tree is NULL.\n");
    //     return 0.0;
    // }
    Sum sum_depth = bst2dAverageNodeDepthRecursive(bst2d->root);

    double average = 0.0;
    //size_t nodeCount = bst2dCountNodes(bst2d->root);
    // LNode *currDepth = all_depth->head;
    // while(currDepth != NULL){
    //     double extractedValue = *(double*)(currDepth->value);
    //     average += extractedValue;
    //     currDepth = currDepth->next;
    // }
    average = sum_depth.sum/sum_depth.nb_nodes;

    // listFree(all_depth,true);
    // free(all_depth);
    return average;
}

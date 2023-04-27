/* ========================================================================= *
 * PointDct2d definition (with BST)
 * ========================================================================= */
#ifndef POINTDCT_H
#define POINTDCT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include "List.h"
#include "PointDct.h"
#include "BST2d.h" // Include the BST2d header file for using the BST data structure

//--------------------------------------------------------------------------------------------------------------------------------

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

struct PointDct_t {
    BST2d *bst2d; // Pointeur vers l'arbre binaire de recherche
};

//--------------------------------------------------------------------------------------------------------------------------------
BST2dNode *bstNodeCreate(void *key, void *value);
void bstBallSearchRec(BST2dNode *node, List *result, Point *center, double radius);
//--------------------------------------------------------------------------------------------------------------------------------

PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = (PointDct *)malloc(sizeof(PointDct));
    if (pd == NULL) {
        printf("Error in pdctCreate: Failed to allocate memory for PointDct\n");
        return NULL;
    }
    if (lpoints == NULL) {
        printf("Error in pdctCreate: lpoints is NULL\n");
        return NULL;
    }
    if (lvalues == NULL) {
        printf("Error in pdctCreate: lvalues is NULL\n");
        return NULL;
    }
    if(lpoints->size != lvalues->size) {
        printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }
    pd->bst2d = bst2dNew();
    if (pd->bst2d == NULL) {
        printf("Error in pdctCreate: Failed to allocate memory for BST2d\n");
        return NULL;
    }

    LNode *currPoint = lpoints->head;
    LNode *currValue = lvalues->head;
    
    if(currPoint == NULL || currValue == NULL) {
        printf("Error in pdctCreate: lpoints or lvalues is empty\n");
        return NULL;
    }
    while (currPoint != NULL && currValue != NULL) {

        bst2dInsert(pd->bst2d, currPoint->value, currValue->value);
        currPoint = currPoint->next;
        currValue = currValue->next;
    }

    // printf("\n");
    // printf("bstroot: x = %ld\n, y = %ld\n", pd->bst2d->root->p->x, pd->bst2d->root->p->y);

    // Check if both lists have the same number of elements
    if (currPoint != NULL || currValue != NULL) {
        printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }
    return pd; // Return the pointer to the created PointDct structure
}


// //--------------------------------------------------------------------------------------------------------------------------------

// BST2dNode *bstNodeCreate(void *key, void *value){
//     BST2dNode *node = (BST2dNode *)malloc(sizeof(BST2dNode));
//     if (node == NULL) {
//         printf("Error in bstNodeCreate: Failed to allocate memory for BNode\n");
//         return NULL;
//     }
    
//     node->left = NULL;
//     node->right = NULL;
//     return node;
// }

//--------------------------------------------------------------------------------------------------------------------------------

void pdctFree(PointDct *pd){
    if (pd == NULL) {
        printf("Warning in pdctFree: pd is already NULL\n");
        return;
    }
    bst2dFree(pd->bst2d, true, true); // Libérer l'arbre binaire de recherche, les clés et les valeurs associées
    free(pd); // Libérer la structure PointDct elle-même
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t pdctSize(PointDct *pd){
    if (pd == NULL) {
        printf("Error in pdctSize: pd is NULL\n");
        return 0;
    }

    // Utiliser la fonction bstSize pour obtenir le nombre de nœuds dans l'arbre binaire de recherche
    return bst2dSize(pd->bst2d);
}

//--------------------------------------------------------------------------------------------------------------------------------

/* ------------------------------------------------------------------------- *
 * Returns the value associated to a point, if it belongs to the PointDct.
 * If several duplicate copies of that point belongs to pd, any one of the
 * values associated to that key is returned.
 *
 * PARAMETERS
 * pd           A valid pointer to a PointDct object
 * p            The point to look for
 *
 * RETURN
 * res          One of the value corresponding to that point. Or NULL if
 *              the point is not present in the BST
 * ------------------------------------------------------------------------- */
void *pdctExactSearch(PointDct *pd, Point *p){
    if (pd == NULL) {
        printf("Error in pdctSearch: pd is NULL\n");
        return NULL;
    }
    if (p == NULL) {
        printf("Error in pdctSearch: p is NULL\n");
        return NULL;
    }

    // Utiliser la fonction bstSearch pour obtenir la valeur associée à la clé p
    void* val = bst2dSearch(pd->bst2d, p);
    if(val == NULL) {
        //printf("Error in pdctSearch: p is not present in the BST\n");
        return NULL;
    }
    return val;
}
// void *pdctExactSearch(PointDct *pd, Point *p)
// {
//     void* val = bst2dSearch(pd->bst2d, p);
//     // printf("node: %p\n", val);
//     if (val != NULL)
//     {
//         return val;
//     }
//     else
//     {
//         return NULL;
//     }
// }

//--------------------------------------------------------------------------------------------------------------------------------

void bstBallSearchRec(BST2dNode *node, List *result, Point *center, double radius)
{
    if (node == NULL)
    {
        return;
    }

    printf("10\n");
    // Calcul de la distance entre la clé du nœud et le centre du ball
    double distance = sqrt(ptSqrDistance(node->p, center));

    printf("11\n");
    // Si la distance est inférieure ou égale au rayon du ball, alors la clé est incluse dans le ball
    if (distance <= radius)
    {
        listInsertLast(result, node->p); // Ajout de la valeur associée à la clé dans la liste résultat
    }

    printf("12\n");

    // Recherche récursive dans les sous-arbres gauche et droit
    bstBallSearchRec(node->left, result, center, radius);
    printf("13\n");
    bstBallSearchRec(node->right, result, center, radius);
}


//--------------------------------------------------------------------------------------------------------------------------------

List *pdctBallSearch(PointDct *pd, Point *q, double r)
{
    // Création d'une nouvelle liste pour stocker les valeurs incluses dans le ball
    List *result = listNew();
    if (result == NULL)
    {
        printf("Error in pdctBallSearch: Failed to allocate memory for List\n");
        return NULL;
    }

    // Parcours de l'arbre binaire de recherche à partir de la racine
    bstBallSearchRec(pd->bst2d->root, result, q, r);

    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------

#endif // POINTDCT_H


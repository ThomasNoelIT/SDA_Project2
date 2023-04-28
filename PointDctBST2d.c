/* ========================================================================= *
 * PointDct2d definition (with BST)
 * ========================================================================= */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "Point.h"
#include "List.h"
#include "PointDct.h"
#include "BST2d.h"

struct PointDct_t {
    BST2d *bst2d; // Pointeur vers l'arbre binaire de recherche
};

//------------------------------------------------------------------------------------------------------------

PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = (PointDct *)malloc(sizeof(PointDct));
    if (pd == NULL) {
        printf("Error in pdctCreate: Failed to allocate memory for PointDct.\n");
        return NULL;
    }
    if (lpoints == NULL) {
        printf("Error in pdctCreate: lpoints is NULL.\n");
        return NULL;
    }
    if (lvalues == NULL) {
        printf("Error in pdctCreate: lvalues is NULL.\n");
        return NULL;
    }
    if(lpoints->size != lvalues->size) {
        printf("Error in pdctCreate: lpoints and lvalues have different number of elements.\n");
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
        //bst2dInsert(pd->bst2d, currPoint, currValue->value);
        currPoint = currPoint->next;
        currValue = currValue->next;
    }

    // Check if both lists have the same number of elements
    if (currPoint != NULL || currValue != NULL) {
        printf("Error in pdctCreate: lpoints and lvalues have different number of elements.\n");
        return NULL;
    }
    return pd; // Return the pointer to the created PointDct structure
}

//--------------------------------------------------------------------------------------------------------------------------------

void pdctFree(PointDct *pd){
    if (pd == NULL) {
        printf("Warning in pdctFree: pd is NULL.\n");
        return;
    }
    bst2dFree(pd->bst2d, false, false); // Libérer l'arbre binaire de recherche, les clés et les valeurs associées
    free(pd); // Libérer la structure PointDct elle-même
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t pdctSize(PointDct *pd){
    if (pd == NULL) {
        printf("Error in pdctSize: pd is NULL.\n");
        return 0;
    }
    // Utiliser la fonction bstSize pour obtenir le nombre de nœuds dans l'arbre binaire de recherche
    return bst2dSize(pd->bst2d);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *pdctExactSearch(PointDct *pd, Point *p){
    if (pd == NULL) {
        printf("Error in pdctSearch: pd is NULL.\n");
        return NULL;
    }
    if (p == NULL) {
        printf("Error in pdctSearch: p is NULL.\n");
        return NULL;
    }
    // Utiliser la fonction bstSearch pour obtenir la valeur associée à la clé p
    void* res = bst2dSearch(pd->bst2d, p);
    //printf("Recherche de la position (%f, %f)\n", ptGetx(p), ptGety(p));
    if(res == NULL) {
        //printf("p is not present in the BST.\n");
        return NULL;
    }
    printf("res = (%f)\n", res);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------------------

List *pdctBallSearch(PointDct *pd, Point *q, double r)
{ 
    return bst2dBallSearch(pd->bst2d, q, r);
}

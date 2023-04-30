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
#include "BST2d.h"

//--------------------------------------------------------------------------------------------------------------------------------
// PointDct structure definition
typedef struct BST2dNode_t BST2dNode;

//structure for the binary search tree
struct BST2d_t {
    BST2dNode *root;
    size_t size;
};

//structure for the dictionary binary search tree
struct PointDct_t {
    BST2d *bst2d; // Pointeur vers l'arbre binaire de recherche
};

//--------------------------------------------------------------------------------------------------------------------------------
// Create a new PointDct structure
PointDct *pdctCreate(List *lpoints, List *lvalues) {
    if (lpoints == NULL) {
        //printf("Error in pdctCreate: lpoints is NULL\n");
        return NULL;
    }
    if (lvalues == NULL) {
        //printf("Error in pdctCreate: lvalues is NULL\n");
        return NULL;
    }
    if(lpoints->size != lvalues->size) {
        //printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }

    PointDct *pd = (PointDct *)malloc(sizeof(PointDct));
    if (pd == NULL) {
        //printf("Error in pdctCreate: Failed to allocate memory for PointDct\n");
        return NULL;
    }

    pd->bst2d = bst2dNew();
    if (pd->bst2d == NULL) {
        //printf("Error in pdctCreate: Failed to allocate memory for BST2d\n");
        return NULL;
    }

    LNode *currPoint = lpoints->head;
    if(currPoint == NULL) {
        //printf("Error in pdctCreate: lpoints is empty\n");
        return NULL;
    }

    LNode *currValue = lvalues->head;
    if(currValue == NULL) {
        //printf("Error in pdctCreate: lvalues is empty\n");
        return NULL;
    }
    
    if(currPoint == NULL || currValue == NULL) {
        //printf("Error in pdctCreate: lpoints or lvalues is empty\n");
        return NULL;
    }

    bool temp;

    while (currPoint != NULL && currValue != NULL) {
        temp = bst2dInsert(pd->bst2d, currPoint->value, currValue->value);
        if(temp == false) {
            //printf("Error in pdctCreate: Failed to insert into BST2d\n");
            return NULL;
        }
        currPoint = currPoint->next;
        currValue = currValue->next;
    }

    // Check if both lists have the same number of elements
    if (currPoint != NULL || currValue != NULL) {
        //printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }
    return pd;
}

//--------------------------------------------------------------------------------------------------------------------------------
// Free a PointDct structure
void pdctFree(PointDct *pd){
    if (pd == NULL) {
        //printf("Warning in pdctFree: pd is already NULL\n");
        return;
    }
    bst2dFree(pd->bst2d, false, false);
    free(pd);
}

//--------------------------------------------------------------------------------------------------------------------------------
// Print a PointDct structure
size_t pdctSize(PointDct *pd){
    if (pd == NULL) {
        printf("Error in pdctSize: pd is NULL\n");
        return 0;
    }
    return bst2dSize(pd->bst2d);
}

//--------------------------------------------------------------------------------------------------------------------------------
// compute the exact search of a point in the dictionary
void *pdctExactSearch(PointDct *pd, Point *p){
    if (pd == NULL) {
        //printf("Error in pdctSearch: pd is NULL\n");
        return NULL;
    }
    if (p == NULL) {
        //printf("Error in pdctSearch: p is NULL\n");
        return NULL;
    }

    void* val = bst2dSearch(pd->bst2d, p);
    if(val == NULL) {
        //printf("Error in pdctSearch: p is not present in the BST\n");
        return NULL;
    }

    return val;
}

//--------------------------------------------------------------------------------------------------------------------------------

List *pdctBallSearch(PointDct *pd, Point *p, double r){
    if (pd == NULL) {
        //printf("Error in pdctBallSearch: pd is NULL\n");
        return NULL;
    }
    if (p == NULL) {
        //printf("Error in pdctBallSearch: p is NULL\n");
        return NULL;
    }
    if (r < 0) {
        //printf("Error in pdctBallSearch: r is negative\n");
        return NULL;
    }
    
    return bst2dBallSearch(pd->bst2d, p, r);
}

#endif // POINTDCT_H

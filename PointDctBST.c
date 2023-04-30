/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */
#ifndef POINTDCT_H
#define POINTDCT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Point.h"
#include "List.h"
#include "PointDct.h"
#include "BST.h"

//--------------------------------------------------------------------------------------------------------------------------------
// PointDct structure definition
struct PointDct_t {
    BST *bst; // Pointer to the binary search tree
};

//--------------------------------------------------------------------------------------------------------------------------------
//compare function for the binary search tree
static int mycompare(void *a, void *b){
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    int cpt = ptCompare(p1, p2);
    return cpt;
}

//--------------------------------------------------------------------------------------------------------------------------------
// Create a new PointDct structure
PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = (PointDct *)malloc(sizeof(PointDct));
    if (pd == NULL) {
        //printf("Error in pdctCreate: Failed to allocate memory for PointDct\n");
        return NULL;
    }
    if (lpoints == NULL) {
        //printf("Error in pdctCreate: lpoints is NULL\n");
        return NULL;
    }
    if (lvalues == NULL) {
        //printf("Error in pdctCreate: lvalues is NULL\n");
        return NULL;
    }
    pd->bst = bstNew(mycompare);

    // Insert the values from lpoints and lvalues into the binary search tree
    LNode *currPoint = lpoints->head;
    LNode *currValue = lvalues->head;
    
    if(currPoint == NULL || currValue == NULL) {
        //printf("Error in pdctCreate: lpoints or lvalues is empty\n");
        return NULL;
    }

    bool temp = 0;
    // Traverse the lists and insert the values into the binary search tree
    while (currPoint != NULL && currValue != NULL) {
        temp = bstInsert(pd->bst, currPoint->value, currValue->value);
        if(temp == 0) {
            //printf("Error in pdctCreate: Failed to insert node into the binary search tree\n");
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
    return pd; // Return the pointer to the created PointDct structure

}

//--------------------------------------------------------------------------------------------------------------------------------
// Free the memory allocated for the PointDct structure
void pdctFree(PointDct *pd){
    if (pd == NULL) {
        //printf("Warning in pdctFree: pd is already NULL\n");
        return;
    }
    bstFree(pd->bst, false, false);
    free(pd);
}

//--------------------------------------------------------------------------------------------------------------------------------
// Return the size of the binary search tree
size_t pdctSize(PointDct *pd){
    if (pd == NULL){
        //printf("Error in pdctSize: pd is NULL\n");
        return 0;
    }
    // Return the size of the binary search tree
    return bstSize(pd->bst);
}

//--------------------------------------------------------------------------------------------------------------------------------
// Return the value associated with the point p
void *pdctExactSearch(PointDct *pd, Point *p){
    void* val = bstSearch(pd->bst, p);
    if (val != NULL){
        return val;
    }
    return NULL;
}

//--------------------------------------------------------------------------------------------------------------------------------
// Return a list of values associated with the points in the rectangle
List *pdctBallSearch(PointDct *pd, Point *p, double r){
    void* val;
    bool sort;
    Point *temp1 = ptNew(ptGetx(p)-r, ptGety(p));
    if(temp1 == NULL){
        //printf("Error in pdctBallSearch: Failed to allocate memory for Point\n");
        return NULL;
    }
    Point *temp2 = ptNew(ptGetx(p)+r, ptGety(p));
    if(temp2 == NULL){
        //printf("Error in pdctBallSearch: Failed to allocate memory for Point\n");
        return NULL;
    }
    List *result = listNew();
    if (result == NULL){
        //printf("Error in pdctBallSearch: Failed to allocate memory for List\n");
        return NULL;
    }

    List* l_point = bstRangeSearch(pd->bst, temp1, temp2); // return a list of points
    if(l_point == NULL){
        //printf("Error in pdctBallSearch: Failed to allocate memory for List\n");
        return NULL;
    }
    LNode *curr = l_point->head;
    
    while(curr != NULL){
        val = bstSearch(pd->bst,curr->value);
        if(val == NULL){
            //printf("Error in pdctBallSearch: Failed to search value in bst\n");
            return NULL;
        }
        if(ptSqrDistance(curr->value, p) <= r*r){
            sort = listInsertLast(result, val);
            if(sort == false){
                //printf("Error in pdctBallSearch: Failed to insert value in list\n");
                return NULL;
            }
        }
        curr = curr->next;
    }

    listFree(l_point, false);
    ptFree(temp1);
    ptFree(temp2);

    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

#endif // POINTDCT_H


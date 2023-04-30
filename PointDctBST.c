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
#include "BST.h" // Include the BST header file for using the BST data structure

//--------------------------------------------------------------------------------------------------------------------------------

// typedef struct Pair{
//     void *key;
//     void *value;
// } Pair;

struct PointDct_t {
    BST *bst; // Pointeur vers l'arbre binaire de recherche
};

//--------------------------------------------------------------------------------------------------------------------------------

// BNode *bstNodeCreate(void *key, void *value);
// List *pdctBallSearch(PointDct *pd, Point *q, double r);
// void bstBallSearchRec(BNode *node, List *result, Point *q, double r);
// PointDct *pdctCreate(List *lpoints, List *lvalues);

static int mycompare(void *a, void *b){
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;
    int cpt = ptCompare(p1, p2);
    return cpt;
}
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
    //pd->bst = bstNew((int f(void *, void*))ptCompare); // Pass NULL as comparison function to bstNew

    // Insert the values from lpoints and lvalues into the binary search tree
    LNode *currPoint = lpoints->head;
    LNode *currValue = lvalues->head;
    
    if(currPoint == NULL || currValue == NULL) {
        //printf("Error in pdctCreate: lpoints or lvalues is empty\n");
        return NULL;
    }

    //bool temp = 0;
    // Traverse the lists and insert the values into the binary search tree
    while (currPoint != NULL && currValue != NULL) {

        // printf("currPoint: %p\n", currPoint->value);
        // printf("currValue: %p\n", currValue->value);
        // printf("currPoint next : %p\n", currPoint->next->value);
        // printf("currValue next : %p\n", currValue->next->value);

        // Create a new node with the key (point) and the associated value
        //printf("20\n");
        bstInsert(pd->bst, currPoint->value, currValue->value);
        // printf("temp: %d\n", temp);
        // printf("21\n");
        // Insert the node into the binary search tree
        //bstInsert(pd->bst, node->key, node->value);
        // Move to the next point and next value in the lists
        currPoint = currPoint->next;
        currValue = currValue->next;

        // if(currPoint == NULL || currValue == NULL) {
        //     printf("Error in pdctCreate: lpoints or lvalues is empty\n");
        //     return NULL;
        // }

        // printf("currPoint: %p\n", currPoint->value);
        // printf("currValue: %p\n", currValue->value);
        
        //printf("22\n");
    }

    // Check if both lists have the same number of elements
    if (currPoint != NULL || currValue != NULL) {
        //printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }
    return pd; // Return the pointer to the created PointDct structure

}

//--------------------------------------------------------------------------------------------------------------------------------

// BNode *bstNodeCreate(void *key, void *value) {
//     BNode *node = (BNode *)malloc(sizeof(BNode));
//     if (node == NULL) {
//         printf("Error in bstNodeCreate: Failed to allocate memory for BNode\n");
//         return NULL;
//     }
//     node->key = key;
//     node->value = value;
//     node->left = NULL;
//     node->right = NULL;
//     return node;
// }

//--------------------------------------------------------------------------------------------------------------------------------

void pdctFree(PointDct *pd) {
    if (pd == NULL) {
        //printf("Warning in pdctFree: pd is already NULL\n");
        return;
    }
    bstFree(pd->bst, false, false); // Libérer l'arbre binaire de recherche, les clés et les valeurs associées
    free(pd); // Libérer la structure PointDct elle-même
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t pdctSize(PointDct *pd) {
    if (pd == NULL) {
        //printf("Error in pdctSize: pd is NULL\n");
        return 0;
    }

    // Utiliser la fonction bstSize pour obtenir le nombre de nœuds dans l'arbre binaire de recherche
    return bstSize(pd->bst);
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
void *pdctExactSearch(PointDct *pd, Point *p)
{
    //printf("\n");
    void* val = bstSearch(pd->bst, p);
    //printf("node: %p\n", val);
    if (val != NULL)
    {
        return val;
    }
    else
    {
        return NULL;
    }
}


//--------------------------------------------------------------------------------------------------------------------------------

List *pdctBallSearch(PointDct *pd, Point *p, double r){
    Point *temp1 = ptNew(ptGetx(p)-r, ptGety(p));
    Point *temp2 = ptNew(ptGetx(p)+r, ptGety(p));
    //List *l_point = listNew();
    // if (l_point == NULL){
    //     printf("Error in pdctBallSearch: Failed to allocate memory for List\n");
    //     return NULL;
    // }
    List *result = listNew();
    if (result == NULL){
        //printf("Error in pdctBallSearch: Failed to allocate memory for List\n");
        return NULL;
    }
    void* val;
    bool sort;

    List* l_point = bstRangeSearch(pd->bst, temp1, temp2); //renvoie une liste de points dans l'intervalle [temp1, temp2]
    LNode *curr = l_point->head;
    
    while(curr != NULL){
        val = bstSearch(pd->bst,curr->value);
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
    // ptFree(curr->value,false);
    ptFree(temp1);
    ptFree(temp2);
    //ptFree(curr->value);
    //free(val);

    return result;
}

//--------------------------------------------------------------------------------------------------------------------------------

#endif // POINTDCT_H


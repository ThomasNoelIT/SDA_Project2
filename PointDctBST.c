/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */
#ifndef POINTDCT_H
#define POINTDCT_H

#include <stdio.h>
#include <stdlib.h>
#include "Point.h"
#include "List.h"
#include "PointDct.h"
#include "BST.h" // Include the BST header file for using the BST data structure

//--------------------------------------------------------------------------------------------------------------------------------

typedef struct bstNode_t {
    void *key;
    void *value; // Nouveau champ pour la valeur associée au nœud
    struct bstNode_t *left;
    struct bstNode_t *right;
} BSTNode;

typedef struct BNode_t BNode;

struct BNode_t
{
    BNode *parent;
    BNode *left;
    BNode *right;
    void *key;
    void *value;
};

struct BST_t
{
    BNode *root;
    size_t size;
    int (*compfn)(void *, void *);
};

struct PointDct_t {
    BST *bst; // Pointeur vers l'arbre binaire de recherche
};

//--------------------------------------------------------------------------------------------------------------------------------

BNode *bstNodeCreate(void *key, void *value);
List *pdctBallSearch(PointDct *pd, Point *q, double r);
void bstBallSearchRec(BNode *node, List *result, Point *q, double r);
PointDct *pdctCreate(List *lpoints, List *lvalues);


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
    pd->bst = bstNew(NULL); // Pass NULL as comparison function to bstNew

    // Insert the values from lpoints and lvalues into the binary search tree
    LNode *currPoint = lpoints->head;
    LNode *currValue = lvalues->head;

    // Traverse the lists and insert the values into the binary search tree
    while (currPoint != NULL && currValue != NULL) {
        // Create a new node with the key (point) and the associated value
        bstInsert(pd->bst, currPoint->value, currValue->value);
        // Insert the node into the binary search tree
        //bstInsert(pd->bst, node->key, node->value);
        // Move to the next point and next value in the lists
        currPoint = currPoint->next;
        currValue = currValue->next;
    }


    // Check if both lists have the same number of elements
    if (currPoint != NULL || currValue != NULL) {
        printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
        return NULL;
    }
return pd; // Return the pointer to the created PointDct structure

}


// // Define the function pdctCreate
// PointDct *pdctCreate(List *lpoints, List *lvalues) {
//     PointDct *pd = (PointDct *)malloc(sizeof(PointDct));
//     if (pd == NULL) {
//         printf("Error in pdctCreate: Failed to allocate memory for PointDict\n");
//         return NULL;
//     }
//     if (lpoints == NULL) {
//         printf("Error in pdctCreate: lpoints is NULL\n");
//         return NULL;
//     }
//     if (lvalues == NULL) {
//         printf("Error in pdctCreate: lvalues is NULL\n");
//         return NULL;
//     }

//     pd->bst = bstNew(NULL); // Pass NULL as comparison function to bstNew

//     // Insérer les valeurs de lpoints et lvalues dans l'arbre binaire de recherche
//     size_t size_pt = listSize(lpoints);
//     bstInsert(pd->bst, (void *) size_pt, lvalues->head->value);

//     size_t size_val = listSize(lvalues);
//     bstInsert(pd->bst, (void *) size_val, lpoints->head->value);

//     BNode *currPoint = lpoints->head;
//     BNode *currValue = lvalues->head;
    

//     // Parcourir les listes et insérer les valeurs dans l'arbre binaire de recherche
//     while (currPoint != NULL && currValue != NULL) {
//         // Créer un nouveau nœud avec la clé (point) et la valeur associée
//         BNode *node = bstInsert(pd->bst,currPoint->value, currValue->value);
//         // Insérer le nœud dans l'arbre binaire de recherche
//         bstInsert(pd->bst, node->key, node->value);
//         // Passer au prochain point et à la prochaine valeur dans les listes
//         currPoint = currPoint->right;
//         currValue = currValue->right;
//     }

//     // Vérifier si les deux listes ont le même nombre d'éléments
//     if (currPoint != NULL || currValue != NULL) {
//         printf("Error in pdctCreate: lpoints and lvalues have different number of elements\n");
//         return NULL;
//     }

//     return pd; // Retourner le pointeur vers la structure PointDct créée
// }

BNode *bstNodeCreate(void *key, void *value) {
    BNode *node = (BNode *)malloc(sizeof(BNode));
    if (node == NULL) {
        printf("Error in bstNodeCreate: Failed to allocate memory for BNode\n");
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL; // Ajouter cette ligne pour initialiser le pointeur parent à NULL
    return node;
}

//--------------------------------------------------------------------------------------------------------------------------------

void pdctFree(PointDct *pd) {
    if (pd == NULL) {
        printf("Warning in pdctFree: pd is already NULL\n");
        return;
    }
    bstFree(pd->bst, true, true); // Libérer l'arbre binaire de recherche, les clés et les valeurs associées
    free(pd); // Libérer la structure PointDct elle-même
}

//--------------------------------------------------------------------------------------------------------------------------------

size_t pdctSize(PointDct *pd) {
    if (pd == NULL) {
        printf("Error in pdctSize: pd is NULL\n");
        return 0;
    }

    // Utiliser la fonction bstSize pour obtenir le nombre de nœuds dans l'arbre binaire de recherche
    return bstSize(pd->bst);
}

//--------------------------------------------------------------------------------------------------------------------------------

void *pdctExactSearch(PointDct *pd, Point *p)
{
    BNode *node = bstSearch(pd->bst, p);
    if (node != NULL)
    {
        return node->value;
    }
    else
    {
        return NULL;
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

void bstBallSearchRec(BNode *node, List *result, Point *q, double r)
{
    if (node == NULL)
    {
        return;
    }

    // Calcul de la distance entre la clé du nœud et le centre du ball
    double distance = ptSqrDistance(node->key, q);

    // Si la distance est inférieure ou égale au rayon du ball, alors la clé est incluse dans le ball
    if (distance <= r)
    {
        listInsertFirst(result, node->value); // Ajout de la valeur associée à la clé dans la liste résultat
    }

    // Recherche récursive dans les sous-arbres gauche et droit
    bstBallSearchRec(node->left, result, q, r);
    bstBallSearchRec(node->right, result, q, r);
}


//--------------------------------------------------------------------------------------------------------------------------------

List *pdctBallSearch(PointDct *pd, Point *q, double r)
{
    // Création d'une nouvelle liste pour stocker les valeurs incluses dans le ball
    List *result = listNew();
    if (!result)
    {
        // Gestion de l'erreur d'allocation mémoire
        printf("Erreur dans pdctBallSearch: échec de l'allocation mémoire pour la liste résultat.\n");
        return NULL;
    }

    // Parcours de l'arbre binaire de recherche à partir de la racine
    bstBallSearchRec(pd->bst->root, result, q, r);

    return result;
}
//--------------------------------------------------------------------------------------------------------------------------------

#endif // POINTDCT_H


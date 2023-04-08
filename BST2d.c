/* ========================================================================= *
 * BST2d definition
 * ========================================================================= */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "BST2d.h"

// A compléter

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

/* ------------------------------------------------------------------------- *
 * Structure for the nodes of the BST2d
 * ------------------------------------------------------------------------- */
typedef struct Node_t {
    Point* point;
    struct Node_t* left;
    struct Node_t* right;
} Node;

struct BST2d_t {
    Node* root;
};

/* ------------------------------------------------------------------------- *
 * Creates a new node with the given point
 * ------------------------------------------------------------------------- */
static Node* newNode(Point* point) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->point = point;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* ------------------------------------------------------------------------- *
 * Inserts a point into the BST2d
 * ------------------------------------------------------------------------- */
void bst2dInsert(BST2d* bst2d, Point* point) {
    Node* node = newNode(point);
    if (bst2d->root == NULL) {
        bst2d->root = node;
        return;
    }
    Node* current = bst2d->root;
    while (true) {
        if (point->x < current->point->x) {
            if (current->left == NULL) {
                current->left = node;
                return;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = node;
                return;
            }
            current = current->right;
        }
    }
}

/* ------------------------------------------------------------------------- *
 * Searches for a point in the BST2d
 * ------------------------------------------------------------------------- */
Point* bst2dSearch(BST2d* bst2d, double x, double y) {
    Node* current = bst2d->root;
    while (current != NULL) {
        if (x == current->point->x && y == current->point->y) {
            return current->point;
        } else if (x < current->point->x) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

/* ------------------------------------------------------------------------- *
 * Finds all points in the BST2d that fall within the specified range
 * ------------------------------------------------------------------------- */
void bst2dRangeSearch(BST2d* bst2d, double x1, double y1, double x2, double y2, List* resultList) {
    if (bst2d->root == NULL) {
        return;
    }
    bst2dRangeSearchHelper(bst2d->root, x1, y1, x2, y2, resultList);
}

/* ------------------------------------------------------------------------- *
 * Helper function for bst2dRangeSearch()
 * ------------------------------------------------------------------------- */
static void bst2dRangeSearchHelper(Node* node, double x1, double y1, double x2, double y2, List* resultList) {
    if (node == NULL) {
        return;
    }
    if (node->point->x >= x1 && node->point->x <= x2 && node->point->y >= y1 && node->point->y <= y2) {
        listAppend(resultList, node->point);
    }
    if (node->point->x > x1) {
        bst2dRangeSearchHelper(node->left, x1, y1, x2, y2, resultList);
    }
    if (node->point->x < x2) {
        bst2dRangeSearchHelper(node->right, x1, y1, x2, y2, resultList);
    }
}

BST2d *bst2dNew(void) {
    BST2d *bst2d = malloc(sizeof(BST2d));
    if (bst2d == NULL) {
        return NULL;
    }
    bst2d->root = NULL;
    return bst2d;
}


void bst2dFree(BST2d *bst2d, bool freeKey, bool freeValue) {
    if (bst2d != NULL) {
        bst2dNodeFree(bst2d->root, freeKey, freeValue);
        free(bst2d);
    }
}


/* Returns the number of elements in the given BST2d. */
size_t bst2dSize(BST2d *bst2d) {
    if (!bst2d) {
        return 0;
    }
    
    return bst2d->size;
}


/* Inserts a new element with the given position-value pair into the BST2d. */
bool bst2dInsert(BST2d *bst2d, Point *point, void *value) {
    if (!bst2d || !point) {
        return false;
    }

    BST2dNode **node = &(bst2d->root);
    while (*node) {
        int cmp = pointCompare((*node)->point, point);
        if (cmp == 0) {
            /* Found existing point, add new value to list */
            listPushBack((*node)->values, value);
            return true;
        } else if (cmp > 0) {
            node = &((*node)->left);
        } else {
            node = &((*node)->right);
        }
    }

    /* Point does not exist in tree, create new node */
    BST2dNode *new_node = (BST2dNode *)malloc(sizeof(BST2dNode));
    if (!new_node) {
        return false;
    }
    new_node->point = point;
    new_node->values = listNew();
    listPushBack(new_node->values, value);
    new_node->left = NULL;
    new_node->right = NULL;

    *node = new_node;
    bst2d->size++;

    return true;
}


// Searches the BST2d for all elements that fall within the given range.
/*List *bst2dRangeSearch(BST2d *bst2d, Point *lower_bound, Point *upper_bound) {
    if (!bst2d || !lower_bound || !upper_bound) {
        return NULL;
    }

    List *result_list = listNew();

    /* Recursively search the tree */
    bst2dRangeSearchHelper(bst2d->root, lower_bound, upper_bound, result_list);

    return result_list;
}

// Recursive helper function for bst2dRangeSearch().
void bst2dRangeSearchHelper(BST2dNode *node, Point *lower_bound, Point *upper_bound, List *result_list) {
    if (!node) {
        return;
    }

    if (pointCompare(node->point, lower_bound) >= 0 && pointCompare(node->point, upper_bound) <= 0) {
        /* Node's point is within the range */
        ListNode *value_node = node->values->head;
        while (value_node) {
            /* Add all values associated with the point to the result list */
            listPushBack(result_list, value_node->data);
            value_node = value_node->next;
        }
    }

    if (pointCompare(node->point, lower_bound) > 0) {
        bst2dRangeSearchHelper(node->left, lower_bound, upper_bound, result_list);
    }

    if (pointCompare(node->point, upper_bound) < 0) {
        bst2dRangeSearchHelper(node->right, lower_bound, upper_bound, result_list);
    }
}*/

// par récursion
void *bst2dSearch(BST2d *bst2d, Point *q) {
    if (bst2d == NULL || pointEqual(q, bst2d->point)) {
        // Si l'arbre est nul ou si le point est trouvé dans l'arbre,
        // on retourne la valeur associée à ce point (ou NULL si l'arbre est nul)
        return (bst2d == NULL) ? NULL : bst2d->value;
    }
    else if (pointLess(q, bst2d->point)) {
        // Si le point est plus petit que le point courant, on cherche
        // récursivement dans le sous-arbre gauche
        return bst2dSearch(bst2d->left, q);
    }
    else {
        // Si le point est plus grand que le point courant, on cherche
        // récursivement dans le sous-arbre droit
        return bst2dSearch(bst2d->right, q);
    }
}


List *bst2dBallSearch(BST2d *bst2d, Point *q, double r) {
    List *result = listNew();  // create a new list to store the results
    if (result == NULL) {  // check for allocation error
        return NULL;
    }
    bst2dBallSearchHelper(bst2d, q, r, result);  // call the helper function
    return result;
}

// Helper function for bst2dBallSearch
static void bst2dBallSearchHelper(BST2dNode *node, Point *q, double r, List *result) {
    if (node == NULL) {
        return;
    }
    if (pointDistance(node->point, q) <= r) {  // if the node is within the ball, add its value to the list
        listAppend(result, node->value);
    }
    if (node->left != NULL && pointGetX(node->point) > pointGetX(q) - r) {
        bst2dBallSearchHelper(node->left, q, r, result);  // recursively search left subtree
    }
    if (node->right != NULL && pointGetX(node->point) < pointGetX(q) + r) {
        bst2dBallSearchHelper(node->right, q, r, result);  // recursively search right subtree
    }
    if (node->top != NULL && pointGetY(node->point) > pointGetY(q) - r) {
        bst2dBallSearchHelper(node->top, q, r, result);  // recursively search top subtree
    }
    if (node->bottom != NULL && pointGetY(node->point) < pointGetY(q) + r) {
        bst2dBallSearchHelper(node->bottom, q, r, result);  // recursively search bottom subtree
    }
}


double bst2dAverageDepth(BST2dNode *node) {
    if (node == NULL) {
        return 0;
    }
    double totalDepth = 0;  // total depth of all nodes
    size_t numNodes = 0;    // number of nodes
    bst2dAverageDepthHelper(node, 0, &totalDepth, &numNodes);
    return totalDepth / numNodes;
}

// Helper function for bst2dAverageDepth
static void bst2dAverageDepthHelper(BST2dNode *node, size_t depth, double *totalDepth, size_t *numNodes) {
    if (node == NULL) {
        return;
    }
    *totalDepth += depth;   // add depth of current node to total depth
    (*numNodes)++;          // increment number of nodes
    bst2dAverageDepthHelper(node->left, depth + 1, totalDepth, numNodes);    // recursively visit left subtree
    bst2dAverageDepthHelper(node->right, depth + 1, totalDepth, numNodes);   // recursively visit right subtree
    bst2dAverageDepthHelper(node->top, depth + 1, totalDepth, numNodes);     // recursively visit top subtree
    bst2dAverageDepthHelper(node->bottom, depth + 1, totalDepth, numNodes);  // recursively visit bottom subtree
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------



/*
// Define a structure for a node in the BST2d
typedef struct BST2dNode_t {
    Point *point;               // Point data
    void *value;                // Associated value
    struct BST2dNode_t *left;   // Pointer to the left subtree
    struct BST2dNode_t *right;  // Pointer to the right subtree
} BST2dNode;

// Define a structure for a BST2d
struct BST2d_t {
    BST2dNode *root;            // Pointer to the root node
    size_t size;                // Number of elements in the BST2d
};

// Private helper function to create a new BST2d node
static BST2dNode *newNode(Point *point, void *value) {
    BST2dNode *node = malloc(sizeof(BST2dNode));
    if (node == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for a new BST2d node\n");
        return NULL;
    }
    node->point = point;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Private helper function to insert a new node recursively
static bool insertNode(BST2dNode **node, Point *point, void *value) {
    // If the node is null, create a new node here
    if (*node == NULL) {
        *node = newNode(point, value);
        return true;
    }
    // Compare the new point with the current node's point
    int cmpx = pointCompareX(point, (*node)->point);
    int cmpy = pointCompareY(point, (*node)->point);
    // If the new point is "less than" or "equal to" the current node's point, go left
    if (cmpx <= 0 && cmpy <= 0) {
        return insertNode(&((*node)->left), point, value);
    }
    // Otherwise, go right
    return insertNode(&((*node)->right), point, value);
}

// Private helper function to search for a node recursively
static void *searchNode(BST2dNode *node, Point *point) {
    // If the node is null, the point is not in the BST2d
    if (node == NULL) {
        return NULL;
    }
    // If the current node's point is equal to the search point, return the node's value
    if (pointEquals(node->point, point)) {
        return node->value;
    }
    // Compare the search point with the current node's point
    int cmpx = pointCompareX(point, node->point);
    int cmpy = pointCompareY(point, node->point);
    // If the search point is "less than" the current node's point, go left
    if (cmpx < 0 || (cmpx == 0 && cmpy < 0)) {
        return searchNode(node->left, point);
    }
    // Otherwise, go right
    return searchNode(node->right, point);
}*/


    
/*#include <stdbool.h>
#include <stdlib.h>

#include "BST2d.h"
#include "Point.h"
#include "List.h"

// A compléter

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
//----------------------------------------------------------------------------------------------------------------------------------------------------

static BST2dNode *bst2dNodeNew(Point *p, bool vertical) {
    BST2dNode *n = malloc(sizeof(BST2dNode));
    if (n == NULL) {
        printf("bst2dNodeNew: allocation error\n");
        return NULL;
    }
    n->p = p;
    n->left = NULL;
    n->right = NULL;
    n->vertical = vertical;
    return n;
}

static void bst2dNodeFree(BST2dNode *n) {
    if (n == NULL) {
        return;
    }
    bst2dNodeFree(n->left);
    bst2dNodeFree(n->right);
    pointFree(n->p);
    free(n);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

BST2d *bst2dNew(void) {
    BST2d *bst = malloc(sizeof(BST2d));
    if (bst == NULL) {
        printf("bst2dNew: allocation error\n");
        return NULL;
    }
    bst->root = NULL;
    bst->size = 0;
    return bst;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------

void bst2dFree(BST2d *bst) {
    bst2dNodeFree(bst->root);
    free(bst);
}

size_t bst2dSize(BST2d *bst) {
    return bst->size;
}

/*static BST2dNode *bst2dInsertRec(BST2dNode *n, Point *p, bool vertical) {
    //if (n == NULL) {
    //    return bst2dNodeNew(p, vertical);
    //}
    if (pointEquals(n->p, p)) {
        return n;
    }
    int cmp = vertical ? pointCompareByX(p, n->p) : pointCompareByY(p, n->p);
    if (cmp < 0) {
        n->left = bst2dInsertRec(n->left, p, !vertical);
    } else {
        n->right = bst2dInsertRec(n->right, p, !vertical);
    }
    return n;
}*/

void bst2dInsert(BST2d *bst, Point *p) {
    bst->root = bst2dInsertRec(bst->root, p, true);
    bst->size++;
}

// static void bst2dRangeSearchRec(BST2dNode *n, Rect rect, List *list) attention !!!*/

                                
//----------------------------------------------------------------------------------------------------------------------------------------------------

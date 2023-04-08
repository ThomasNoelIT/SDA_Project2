/* ========================================================================= *
 * PointDct definition (with BST)
 * ========================================================================= */

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST.h"

// A compléter

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include <stdlib.h>
#include "PointDct.h"

typedef struct TreeNode_t TreeNode;

struct TreeNode_t {
    Point *point;
    void *value;
    TreeNode *left;
    TreeNode *right;
};

struct PointDct_t {
    TreeNode *root;
};

/* ------------------------------------------------------------------------- *
 * Static helper function to recursively free tree nodes and their data
 * ------------------------------------------------------------------------- */

static void freeNode(TreeNode *node) {
    if (node != NULL) {
        freeNode(node->left);
        freeNode(node->right);
        free(node);
    }
}

/* ------------------------------------------------------------------------- *
 * Creates a new tree node with the given point and value, and returns a
 * pointer to it.
 * ------------------------------------------------------------------------- */

static TreeNode *createNode(Point *point, void *value) {
    TreeNode *node = malloc(sizeof(TreeNode));
    node->point = point;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* ------------------------------------------------------------------------- *
 * Recursively inserts a new node with the given point and value into the
 * tree rooted at the given node.
 * ------------------------------------------------------------------------- */

static void insertNode(TreeNode *node, Point *point, void *value) {
    if (point->x < node->point->x) {
        if (node->left == NULL) {
            node->left = createNode(point, value);
        } else {
            insertNode(node->left, point, value);
        }
    } else if (point->x > node->point->x) {
        if (node->right == NULL) {
            node->right = createNode(point, value);
        } else {
            insertNode(node->right, point, value);
        }
    }
}

/* ------------------------------------------------------------------------- *
 * Creates a PointDict object and inserts each point-value pair from the
 * given lists into the tree.
 * ------------------------------------------------------------------------- */

PointDct *pdctCreate(List *lpoints, List *lvalues) {
    PointDct *pd = malloc(sizeof(PointDct));
    pd->root = NULL;
    ListElmt *p = listHead(lpoints);
    ListElmt *v = listHead(lvalues);
    while (p != NULL && v != NULL) {
        insertNode(pd->root, (Point *) listData(p), listData(v));
        p = listNext(p);
        v = listNext(v);
    }
    return pd;
}

/* ------------------------------------------------------------------------- *
 * Recursively searches for the node with the given x coordinate in the
 * tree rooted at the given node. Returns a pointer to the value associated
 * with the node, or NULL if not found.
 * ------------------------------------------------------------------------- */

static void *findValue(TreeNode *node, double x) {
    if (node == NULL) {
        return NULL;
    } else if (x < node->point->x) {
        return findValue(node->left, x);
    } else if (x > node->point->x) {
        return findValue(node->right, x);
    } else {
        return node->value;
    }
}

/* ------------------------------------------------------------------------- *
 * Returns a pointer to the value associated with the point at the given
 * x coordinate in the dictionary, or NULL if not found.
 * ------------------------------------------------------------------------- */

void *pdctFind(PointDct *pd, double x) {
    return findValue(pd->root, x);
}

//------------------------------------------------------------------------
void pdctFree(PointDct *pd) {
    if (pd == NULL) {
        return;
    }

    pdctFree(pd->left);
    pdctFree(pd->right);
    free(pd);
}

int pdctSize(PointDct *pd) {
    if (pd == NULL) {
        return 0;
    }

    return pdctSize(pd->left) + pdctSize(pd->right) + 1;
}


/*void *pdctGetValue(PointDct *pd, Point *p) {
    if (!pd || !p) {
        return NULL;
    }

    TreeNode *node = bstFind(pd->bst, p);
    if (node) {
        return node->value;
    } else {
        return NULL;
    }
}*/

void *pdctExactSearch(PointDct *pd, Point *p) {
    if (!pd || !p) {
        return NULL;
    }
    BSTNode *node = bstExactSearch(pd->bst, p);
    return (node != NULL) ? node->value : NULL;
}


/* ------------------------------------------------------------------------- *
 * Finds the set of positions (x,y) in the Point dictionary that are included
 * in a ball of radius r and centered at the position q given as argument.
 * The function returns a list of the values associated to these positions
 * (in no particular order).
 *
 * PARAMETERS
 * pd           A valid pointer to a PointDct object
 * q            The center of the ball
 * r            The radius of the ball
 *
 * RETURN
 * l            A list containing the values in the given ball, or NULL
 *              in case of allocation error.
 *
 * NOTES
 * The list must be freed but not its content. If no elements are in the ball,
 * the function returns an empty list.
 * ------------------------------------------------------------------------- */
/*List *pdctBallSearch(PointDct *pd, Point *p, double r) {
    List *result = listCreate(NULL);
    if (result == NULL) {
        return NULL;
    }

    bstBallSearch(pd->bst, p, r, result);
    return result;
}*/

List pdctBallSearch(BSTree t, double x, double y, double radius) {
    List result = newList();
    pdctBallSearchHelper(t->root, x, y, radius, result);
    return result;
}

void pdctBallSearchHelper(Node *n, double x, double y, double radius, List result) {
    if (n == NULL) {
        return;
    }
    if (distance(n->value->x, n->value->y, x, y) <= radius) {
        insertList(result, n->value);
    }
    if (n->left != NULL && n->left->max_x >= x - radius) {
        pdctBallSearchHelper(n->left, x, y, radius, result);
    }
    if (n->right != NULL && n->right->min_x <= x + radius) {
        pdctBallSearchHelper(n->right, x, y, radius, result);
    }
}




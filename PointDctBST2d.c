/* ========================================================================= *
 * PointDct definition (with BST2d)
 * ========================================================================= */
#include <stdio.h>
#include <stdlib.h>

#include "PointDct.h"
#include "List.h"
#include "Point.h"
#include "BST2d.h"

struct PointDct_t {
    BST2d *bst;
    int size;
};

/* ------------------------------------------------------------------------- *
 * Creates a PointDict object (dictionary of points). Points and values
 * are expected to be stored by pointers in the structure. They will not
 * be freed by pdctFree.
 *
 * PARAMETERS
 * lpoints          A list of Point objects (Point pointers)
 * lvalues          A list of values (void * pointers)
 *
 * RETURN
 * pd               A PointDict object
 * ------------------------------------------------------------------------- */

PointDct *pdctCreate(List *lpoints, List *Lvalues){
    PointDct *pd = malloc(sizeof(PointDct));
    pd->bst = bst2dCreate();
    pd->size = 0;
    List *lp = lpoints;
    List *lv = Lvalues;
    while (lp != NULL && lv != NULL) {
        bst2dInsert(pd->bst, lp->value, lv->value);
        lp = lp->next;
        lv = lv->next;
        pd->size++;
    }
    return pd;
}

/* ------------------------------------------------------------------------- *
 * Frees a PointDct object. The Point objects and values are not freed.
 *
 * PARAMETERS
 * pd            A valid pointer to a PointDct object
 *
 * ------------------------------------------------------------------------- */

void pdctFree(PointDct *pd){
    bst2dFree(pd->bst);
    free(pd);
}

/* ------------------------------------------------------------------------- *
 * Returns the number of points stored in the PointDct object.
 *
 * PARAMETERS
 * pd           A valid pointer to a PointDct object
 *
 * RETURN
 * nb           The number of positions in pd
 * ------------------------------------------------------------------------- */

size_t pdctSize(PointDct *pd){
    return pd->size;
}

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

void *pdctExactSearch(PointDct *pd, Point *p);

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

List *pdctBallSearch(PointDct *pd, Point *p, double r){

}
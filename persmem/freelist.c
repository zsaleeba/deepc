/*
 * The free list is a red black tree of free memory blocks. The 
 * list is stored in the free blocks themselves. Since the "payload"
 * is actually the address of the block we don't need to store the
 * payload explicitly - we already know the address of the block.
 *
 * This code is derived from Julienne Walker's red black tree code.
 */

#include <stdbool.h>

#include "persmem.h"
#include "persmem_internal.h"


/* Checks the colour of a red black tree node. */
#define IS_RED(n) ((n) != NULL && (n)->isRed)


/**
  <summary>
  Performs a single red black rotation in the specified direction
  This function assumes that all nodes are valid for a rotation
  <summary>
  <param name="root">The original root to rotate around</param>
  <param name="dir">The direction to rotate (0 = left, 1 = right)</param>
  <returns>The new root ater rotation</returns>
  <remarks>For jsw_rbtree.c internal use only</remarks>
*/
static pmFreeListNode *singleRotate(pmFreeListNode *root, bool dir)
{
    pmFreeListNode *save = root->link[!dir];

    root->link[!dir] = save->link[dir];
    save->link[dir] = root;

    root->isRed = true;
    save->isRed = false;

    return save;
}


/**
  <summary>
  Performs a double red black rotation in the specified direction
  This function assumes that all nodes are valid for a rotation
  <summary>
  <param name="root">The original root to rotate around</param>
  <param name="dir">The direction to rotate (0 = left, 1 = right)</param>
  <returns>The new root after rotation</returns>
  <remarks>For jsw_rbtree.c internal use only</remarks>
*/
static pmFreeListNode *doubleRotate(pmFreeListNode *root, bool dir)
{
    root->link[!dir] = doubleRotate(root->link[!dir], !dir);

    return singleRotate(root, dir);
}


/**
  <summary>
  Creates an initializes a new red black node with a copy of
  the data. This function does not insert the new node into a tree
  <summary>
  <param name="tree">The red black tree this node is being created for</param>
  <param name="data">The data value that will be stored in this node</param>
  <returns>A pointer to the new node</returns>
  <remarks>
  For jsw_rbtree.c internal use only. The data for this node must
  be freed using the same tree's rel function. The returned pointer
  must be freed using C's free function
  </remarks>
*/
static void initNode(pmFreeListNode *node)
{
    node->isRed = true;
    node->link[0] = NULL;
    node->link[1] = NULL;
}


void pmFreeListInit(pmFreeList *fl)
{
    fl->root = NULL;
    fl->size = 0;
}


void pmFreeListInsert(pmFreeList *fl, void *mem)
{
    if (fl->root == NULL)
    {
        /* We have an empty tree; attach the new node directly to the root. */
        initNode(mem);
        fl->root = mem;
    }
    else 
    {
        pmFreeListNode head = {0}; /* False tree root. */
        pmFreeListNode *g, *t;     /* Grandparent & parent. */
        pmFreeListNode *p, *q;     /* Iterator & parent. */
        int dir = 0, last = 0;

        /* Set up our helpers. */
        t = &head;
        g = p = NULL;
        q = fl->root;
        t->link[1] = fl->root;

        /* Search down the tree for a place to insert. */
        for ( ; ; ) 
        {
            if (q == NULL)
            {
                /* Insert a new node at the first null link. */
                initNode(mem);
                q = mem;
                p->link[dir] = q;
            }
            else if (IS_RED(q->link[0]) && IS_RED(q->link[1])) 
            {
                /* Simple red violation: color flip. */
                q->isRed = true;
                q->link[0]->isRed = false;
                q->link[1]->isRed = false;
            }

            if (IS_RED(q) && IS_RED(p)) 
            {
                /* Hard red violation: rotations necessary */
                bool dir2 = t->link[1] == g;

                if (q == p->link[last])
                {
                    t->link[dir2] = singleRotate(g, !last);
                }
                else
                {
                    t->link[dir2] = doubleRotate(g, !last);
                }
            }

            /* Stop working if we inserted a node. This check also disallows duplicates in the tree. */
            if (q == mem)
                break;

            last = dir;
            dir = (void *)q < mem;

            /* Move the helpers down */
            if (g != NULL)
            {
                t = g;
            }

            g = p, p = q;
            q = q->link[dir];
        }

        /* Update the root (it may be different). */
        fl->root = head.link[1];
    }

    /* Make the root black for simplified logic. */
    fl->root->isRed = false;
    fl->size++;
}


void pmFreeListRemove(pmFreeList *fl, void *mem)
{
    pmFreeListNode head = {0}; /* False tree root. */
    pmFreeListNode *q, *p, *g; /* Helpers. */
    pmFreeListNode *f = NULL;  /* Found item. */
    bool dir = true;

    /* Can't remove from an empty tree. */
    if (fl->root == NULL)
        return;
    
    /* Set up our helpers. */
    q = &head;
    g = p = NULL;
    q->link[1] = fl->root;

    /* Search and push a red node down to fix red violations as we go. */
    while (q->link[dir] != NULL)
    {
        bool last = dir;

        /* Move the helpers down. */
        g = p;
        p = q;
        q = q->link[dir];
        dir = (void *)q < mem;

        /* Save the node with matching data and keep going; we'll do removal tasks at the end. */
        if (q == mem)
        {
            f = q;
        }

        /* Push the red node down with rotations and color flips. */
        if (!IS_RED(q) && !IS_RED(q->link[dir]))
        {
            if (IS_RED(q->link[!dir]))
            {
                p = p->link[last] = singleRotate(q, dir);
            }
            else if (!IS_RED(q->link[!dir]))
            {
                pmFreeListNode *s = p->link[!last];

                if (s != NULL) 
                {
                    if (!IS_RED(s->link[!last]) && !IS_RED(s->link[last]))
                    {
                        /* Color flip. */
                        p->isRed = false;
                        s->isRed = true;
                        q->isRed = true;
                    }
                    else 
                    {
                        bool dir2 = g->link[1] == p;

                        if (IS_RED(s->link[last]))
                            g->link[dir2] = doubleRotate(p, last);
                        else if (IS_RED(s->link[!last]))
                            g->link[dir2] = singleRotate(p, last);

                        /* Ensure correct coloring. */
                        q->isRed = g->link[dir2]->isRed = true;
                        g->link[dir2]->link[0]->isRed = false;
                        g->link[dir2]->link[1]->isRed = false;
                    }
                }
            }
        }
    }

    /* Replace and remove the saved node. */
    if ( f != NULL ) 
    {
        p->link[p->link[1] == q] = q->link[q->link[0] == NULL];
    }

    /* Update the root (it may be different). */
    fl->root = head.link[1];

    /* Make the root black for simplified logic. */
    if (fl->root != NULL)
    {
        fl->root->isRed = false;
    }

    fl->size--;
}


void *pmFreeListRemoveFirst(pmFreeList *fl)
{
    pmFreeListNode *first = fl->root;

    /* Free list is empty. */
    if (first == NULL)
        return NULL;
    
    /* Descend to the left. */
    while (first->link[0] != NULL)
    {
        first = first->link[0];
    }

    /* Remove it. */
    pmFreeListRemove(fl, (void *)first);
    return (void *)first;
}

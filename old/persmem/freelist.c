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

/* The two directions from a node. */
#define LEFT_CHILD 0
#define RIGHT_CHILD 1


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
    root->link[!dir] = singleRotate(root->link[!dir], !dir);

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
    node->link[LEFT_CHILD] = NULL;
    node->link[RIGHT_CHILD] = NULL;
}


void persmemFreeListInit(pmFreeList *fl)
{
    fl->root = NULL;
    fl->size = 0;
}


void persmemFreeListInsert(pmFreeList *fl, void *mem)
{
    if (fl->root == NULL)
    {
        /* We have an empty tree; attach the new node directly to the root. */
        initNode(mem);
        fl->root = mem;
    }
    else 
    {
        pmFreeListNode head = { {NULL, NULL}, 0}; /* False tree root. */
        pmFreeListNode *grandParent, *parent;     /* Grandparent & parent. */
        pmFreeListNode *pos, *posParent;     /* Iterator & parent. */
        int dir = 0, last = 0;

        /* Set up our helpers. */
        parent = &head;
        grandParent = NULL;
        pos = NULL;
        posParent = fl->root;
        parent->link[RIGHT_CHILD] = fl->root;

        /* Search down the tree for a place to insert. */
        while (true)
        {
            if (posParent == NULL)
            {
                /* Insert a new node at the first null link. */
                initNode(mem);
                posParent = mem;
                pos->link[dir] = posParent;
            }
            else if (IS_RED(posParent->link[LEFT_CHILD]) && IS_RED(posParent->link[RIGHT_CHILD])) 
            {
                /* Simple red violation: color flip. */
                posParent->isRed = true;
                posParent->link[LEFT_CHILD]->isRed = false;
                posParent->link[RIGHT_CHILD]->isRed = false;
            }

            if (IS_RED(posParent) && IS_RED(pos)) 
            {
                /* Hard red violation: rotations necessary */
                bool dir2 = parent->link[RIGHT_CHILD] == grandParent;

                if (posParent == pos->link[last])
                {
                    parent->link[dir2] = singleRotate(grandParent, !last);
                }
                else
                {
                    parent->link[dir2] = doubleRotate(grandParent, !last);
                }
            }

            /* Stop working if we inserted a node. This check also disallows duplicates in the tree. */
            if (posParent == mem)
                break;

            last = dir;
            dir = (void *)posParent < mem;

            /* Move the helpers down */
            if (grandParent != NULL)
            {
                parent = grandParent;
            }

            grandParent = pos, pos = posParent;
            posParent = posParent->link[dir];
        }

        /* Update the root (it may be different). */
        fl->root = head.link[RIGHT_CHILD];
    }

    /* Make the root black for simplified logic. */
    fl->root->isRed = false;
    fl->size++;
}


void persmemFreeListRemove(pmFreeList *fl, void *mem)
{
    pmFreeListNode head = { {NULL, NULL}, 0 }; /* False tree root. */
    pmFreeListNode *pos, *parent, *grandParent; /* Helpers. */
    pmFreeListNode *found = NULL;  /* Found item. */
    bool dir = true;

    /* Can't remove from an empty tree. */
    if (fl->root == NULL)
        return;
    
    /* Set up our helpers. */
    pos = &head;
    grandParent = NULL;
    parent = NULL;
    pos->link[RIGHT_CHILD] = fl->root;

    /* Search and push a red node down to fix red violations as we go. */
    while (pos->link[dir] != NULL)
    {
        bool last = dir;

        /* Move the helpers down. */
        grandParent = parent;
        parent = pos;
        pos = pos->link[dir];
        dir = (void *)pos < mem;

        /* Save the node with matching data and keep going; we'll do removal tasks at the end. */
        if (pos == mem)
        {
            found = pos;
        }

        /* Push the red node down with rotations and color flips. */
        if (!IS_RED(pos) && !IS_RED(pos->link[dir]))
        {
            if (IS_RED(pos->link[!dir]))
            {
                parent = parent->link[last] = singleRotate(pos, dir);
            }
            else if (!IS_RED(pos->link[!dir]))
            {
                pmFreeListNode *sibling = parent->link[!last];

                if (sibling != NULL) 
                {
                    if (!IS_RED(sibling->link[!last]) && !IS_RED(sibling->link[last]))
                    {
                        /* Color flip. */
                        parent->isRed = false;
                        sibling->isRed = true;
                        pos->isRed = true;
                    }
                    else 
                    {
                        bool dir2 = grandParent->link[RIGHT_CHILD] == parent;

                        if (IS_RED(sibling->link[last]))
                            grandParent->link[dir2] = doubleRotate(parent, last);
                        else if (IS_RED(sibling->link[!last]))
                            grandParent->link[dir2] = singleRotate(parent, last);

                        /* Ensure correct coloring. */
                        pos->isRed = grandParent->link[dir2]->isRed = true;
                        grandParent->link[dir2]->link[LEFT_CHILD]->isRed = false;
                        grandParent->link[dir2]->link[RIGHT_CHILD]->isRed = false;
                    }
                }
            }
        }
    }

    /* Replace and remove the saved node. */
    if (found != NULL) 
    {
        parent->link[parent->link[RIGHT_CHILD] == pos] = pos->link[pos->link[LEFT_CHILD] == NULL];
    }

    /* Update the root (it may be different). */
    fl->root = head.link[RIGHT_CHILD];

    /* Make the root black for simplified logic. */
    if (fl->root != NULL)
    {
        fl->root->isRed = false;
    }

    fl->size--;
}


void *persmemFreeListRemoveFirst(pmFreeList *fl)
{
    pmFreeListNode *first = fl->root;

    /* Free list is empty. */
    if (first == NULL)
        return NULL;
    
    /* Descend to the left. */
    while (first->link[LEFT_CHILD] != NULL)
    {
        first = first->link[LEFT_CHILD];
    }

    /* Remove it. */
    persmemFreeListRemove(fl, (void *)first);
    return (void *)first;
}



/**
  <summary>
  Search for a copy of the specified
  node data in a red black tree
  <summary>
  <param name="tree">The tree to search</param>
  <param name="data">The data value to search for</param>
  <returns>
  A pointer to the data value stored in the tree,
  or a null pointer if no data could be found
  </returns>
*/
bool persmemFreeListFind(pmFreeList *fl, void *mem)
{
    pmFreeListNode *pos = fl->root;
    
    while (pos != NULL) 
    {
        if (pos == mem)
            return true;
        
        pos = pos->link[(void *)pos < mem];
    }
    
    return false;
}

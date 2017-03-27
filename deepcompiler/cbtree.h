/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                      Source file collection                        ***
 ***                                                                    ***
 ***                                         - Zik Saleeba              ***
 ***                                           2017-03-27               ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_CBTREE_H
#define DEEPC_CBTREE_H

#include <stddef.h>

//
// This module defines a "counted-only" B+-tree. This is an
// auto-balanced tree which is accessed like an array, using  
// array-like indices to address data. Unlike an array however
// the structure can be easily expanded and contracted using
// insertion and deletion operations to any part of the array.
// These operations are are quick, taking O(log n) time compared
// to O(n) for a conventional array. On the other hand data is 
// accessed with O(log n) time, which is slower than an array's 
// constant access time.
//
// This variant on the counted B+-tree doesn't use keys at all,
// hence the "counted-only" name. Also, the values we store
// are buffers which each have more than one addressable element 
// stored in them, so we keep the number of elements below each 
// node in the node in addition to the usual B+-tree structure. This
// allows us to track this extra information.
//
// This implementation allows the order of the tree to
// be adjusted for tuning purposes. The larger the order of the
// tree the higher the performance will be, but also the greater
// the amount of wasted space. Increasing the order makes the
// tree shallower, but also makes the linear operations on 
// individual nodes take longer so some balancing of these two
// factors is necessary for the best performance. Using higher
// orders also has a second effect of taking advantage of cache
// coherency for added performance.
//
// Counted B-trees are described here:
// http://www.chiark.greenend.org.uk/~sgtatham/algorithms/cbtree.html
//

namespace DeepC
{
    //
    // A generic node of a counted B+-tree
    //
    // Nodes can be either "leaf" or "branch" (internal nodes).
    // This class serves as a superclass for the two types of node.
    //
    // In this template:
    //  T is the data type of the values to store
    //  order is the maximum number of children in the B+-tree
    //
    
    template <class T, int order>
    class cbnode
    {
    
        // a pointer which can pointer to either a subtree or a value.
        // these are used to point down the tree
        union child_ptr
        {
            cbnode<T, order> *subtree;  // subtrees of a branch
            T *value;                   // values in a leaf
        };
        
        int num_entries;        // the number of values or subtrees in this node
        bool is_leaf;           // true if this node is a leaf
        size_t total_size;      // size of all the elements in this node and all sub-nodes
        size_t offset[order];   // the offsets of each value within the node
        child_ptr p[order];     // subtrees of a branch or values in a leaf
        
        cbnode<T, order> *prev; // the previous leaf in the linked list
        cbnode<T, order> *next; // the next leaf in the linked list
    
    private:

        size_t entry_size(int entry)
        {
            if (entry == num_entries-1)
                return total_size - offset[entry];
            else
                return offset[entry+1] - offset[entry];
        }
        
        //
        // NAME:        search_node
        // ACTION:      Search the node for an offset.
        //              This may find the offset in a subtree or if it's a leaf
        //              it will find the value the offset refers to. Note that
        //              no out-of-bounds check is done - if the offset is out
        //              of bounds the last subtree will be returned.
        // PARAMETERS:  size_t find_offset - the index of the thing we're looking 
        //                  for in the node
        // RETURNS:     int - the index of the found entry
        //
        
        int search_node(size_t find_offset)
        {
            // binary search within the node
            int left_entry = 0;
            int right_entry = num_entries;
            
            while (right_entry - left_entry > 1)
            {
                int mid_entry = (left_entry + right_entry) >> 1;
                
                if (find_offset < offset[mid_entry])
                    right_entry = mid_entry;
                else
                    left_entry = mid_entry;
            }
            
            return left_entry;
        }
        
        // assumes that both will fit in the one node
        void move_contents(cbnode<T, order> *from_node)
        {
            // move the values across
            memcpy(&p[num_entries], &from_node->p[0], sizeof(from_node->p[0]) * from_node->num_entries);
            
            // move the offsets across and adjust
            for (int count = 0; count < from_node->num_entries; count++)
            {
                offset[num_entries + count] = from_node->offset[count] + total_size;
            }
            
            // fix sizes
            total_size += from_node->total_size;
            num_entries += from_node->num_entries;
            from_node->total_size = 0;
            from_node->num_entries = 0;
        }
        
        cbnode<T, order> *split_node(bool insert_left)
        {
            cbnode<T, order> *right_node = new cbnode<T, order>(is_leaf);
            
            // where will we split the node?
            int split_entry;
            
            if (insert_left)
                split_entry = num_entries/2;                // move more data to the right
            else
                split_entry = num_entries - num_entries/2;  // leave more data on the left
            
            // adjust sizes and entries
            right_node->num_entries = num_entries - split_entry;
            right_node->total_size = total_size - offset[split_entry];
            num_entries = split_entry;
            total_size = offset[split_entry];
            
            // copy values/subtrees across
            memcpy(&right_node->p[0], &p[split_entry], sizeof(p[split_entry]) * right_node->num_entries);

            // copy offsets across and adjust
            for (int count = 0; count < right_node->num_entries; count++)
            {
                right_node->offset[count] = offset[count+split_entry] - total_size;
            }
            
            // link the new node in
//            if (is_leaf)
//            XXX
            
            return right_node;
        }
        
        void insert_entry(int new_entry, child_ptr new_value, size_t new_value_size)
        {
            // there's room - find where we're going to insert it
            if (new_entry >= num_entries)
            {
                // append it to the node
                p[num_entries] = new_value;
            }
            else
            {
                // insert into the node - copy all the values to the right 
                // across by one
                memmove(&p[new_entry+1], &p[new_entry], sizeof(p[new_entry]) * (num_entries - new_entry));
                p[new_entry] = new_value;
                
                // copy all the offsets to the right and adjust them
                for (int count = new_entry; count < num_entries; count++)
                {
                    offset[count+1] = offset[count] + new_value_size;
                }
            }
                
            // set the new offset
            if (new_entry == 0)
                offset[new_entry] = 0;
            else
                offset[new_entry] = offset[new_entry-1] + new_value_size;
            
            num_entries++;
            total_size += new_value_size;
        }
        
        cbnode<T, order> *insert_entry_checked(int new_entry, child_ptr new_value, size_t new_value_size)
        {
            // is there enough room to fit it in the node?
            if (num_entries < order)
            {
                // there's room - insert it
                insert_entry(new_entry, new_value, new_value_size);
                
                // there's nothing that we need inserted up in the parent
                return NULL;
            }
            else
            {
                // we have to split the node in two
                bool insert_left = new_entry <= num_entries/2;
                cbnode<T, order> *right_branch = split_node(insert_left);
                
                if (insert_left)
                {
                    // insert in the left node
                    insert_entry(new_entry, new_value, new_value_size);
                }
                else
                {
                    // insert in the right node
                    right_branch->insert_entry(new_entry - num_entries, new_value, new_value_size);
                }
                
                return right_branch;
            }
        }
        
        child_ptr delete_entry(int entry, size_t &deleted_size)
        {
            child_ptr deleted_value = p[entry];
            
            // how much are we deleting?
            deleted_size = entry_size(entry);
            
            // move the values across
            memmove(&p[entry], &p[entry+1], sizeof(p[entry]) * (num_entries - entry - 1));
            
            // move the offsets across and adjust them
            for (int count = entry; count < num_entries-1; count++)
            {
                offset[count] = offset[count+1] - deleted_size;
            }

            total_size -= deleted_size;
            num_entries--;
            
            return deleted_value;
        }
        
        // should only be called on branches
        void rebalance(int underweight_entry)
        {
            cbnode<T, offset> *underweight_subtree = p[underweight_entry].subtree;
            
            // is there an entry to the left of the underweight one?
            if (underweight_entry > 0)
            {
                // yes, can we steal an item from the left?
                cbnode<T, offset> *left_subtree = p[underweight_entry-1].subtree;
                if (left_subtree->num_entries > ((order+1)/2))
                {
                    // yes, steal from the left
                    size_t stolen_size;
                    child_ptr steal_value = left_subtree->delete_entry(left_subtree->num_entries-1, stolen_size);
                    underweight_subtree->insert_entry(0, steal_value, stolen_size);
                    offset[underweight_entry] += stolen_size;
                }
                else
                {
                    // no, but we can add the contents of this node to the node on the left
                    left_subtree->move_contents(underweight_subtree);
                    
                    // remove the underweight node
                    delete underweight_subtree;
                    delete_entry(underweight_entry);
                }
            }
            else if (underweight_entry+1 < num_entries)
            {
                // can we steal an item from the right instead?
                cbnode<T, offset> *right_subtree = p[underweight_entry+1].subtree;
                if (right_subtree->num_entries > ((order+1)/2))
                {
                    // yes, steal from the right
                    size_t stolen_size;
                    child_ptr steal_value = right_subtree->delete_entry(0, stolen_size);
                    underweight_subtree->insert_entry(num_entries, steal_value, stolen_size);
                    offset[underweight_entry] += stolen_size;
                }
                else
                {
                    // no, but we can add the contents of this node to the node on the right
                    right_subtree->move_contents(underweight_subtree);
                    
                    // remove the underweight node
                    delete underweight_subtree;
                    delete_entry(underweight_entry);
                }
            }
        }

    public:
        cbnode() : num_entries(0), is_leaf(false), total_size(0) {};
        cbnode(bool p_is_leaf) : num_entries(0), is_leaf(p_is_leaf), total_size(0) {};
        
        size_t size() { return total_size; };
        
        //
        // NAME:        branch node constructor
        // ACTION:      Constructs a branch node from insert information passed up
        //              from a call to insert(). We need to create a new root
        //              node based on that information.
        // PARAMETERS:  cbnode<T, order> *left, cbnode<T, order> *right 
        //                  - the information to put in the new node
        //
        
        cbnode(cbnode<T, order> *left, cbnode<T, order> *right)
            : num_entries(2), is_leaf(false)
        {
            total_size = left->total_size + right->total_size;
            p[0].subtree = left;
            p[1].subtree = right;
            offset[0] = 0;
            offset[1] = left->total_size;
        }
        
        ~cbnode()
        {
            // delete all the subtrees
            if (!is_leaf)
            {
                for (int count = 0; count < num_entries; count++)
                    delete p[count].subtree;
            }
        }
        
        
        void link_insert_right(cbnode<T, order> *new_node)
        {
            new_node->prev = this;
            new_node->next = this->next;
            this->next = new_node;
            
            if (new_node->next != NULL)
                new_node->next->prev = new_node;
        }
        
        void link_delete()
        {
            if (this->prev != NULL)
                this->prev->next = this->next;
            
            if (this->next != NULL)
                this->next->prev = this->prev;
        }

        
        //
        // NAME:        lookup
        // ACTION:      Look up an offset.
        //              Finds the value containing the given offset. Also
        //              returns the offset of the found object in the tree.
        // PARAMETERS:  size_t offset - the offset to look for
        //              size_t &found_offset - set to the base offset of the
        //                  found value
        // RETURNS:     T * - the found value or NULL if not found
        //
        
        T *lookup(size_t lookup_offset, size_t &found_offset)
        {
            // check for an out-of-bounds offset
            if (lookup_offset >= total_size)
                return NULL;
                
            // search down the tree until we find a leaf
            cbnode<T, order> *search_subtree = this;
            size_t search_offset = 0;
            while (!search_subtree->is_leaf)
            {
                int branch_entry = search_subtree->search_node(lookup_offset - search_offset);
                search_offset += search_subtree->offset[branch_entry];
                search_subtree = search_subtree->p[branch_entry].subtree;
            }
            
            // find the value we're looking for in the leaf
            int leaf_entry = search_subtree->search_node(lookup_offset - search_offset);
            found_offset = search_offset + search_subtree->offset[leaf_entry];
            return search_subtree->p[leaf_entry].value;
        }
        

        cbnode<T, order> *insert(size_t insert_offset, T *new_value, size_t new_value_size)
        {
            if (is_leaf)
            {
                // find where it should go
                int found_entry;
                if (insert_offset >= total_size)
                    found_entry = num_entries;  // it'll go right at the end
                else
                    found_entry = search_node(insert_offset);

                // insert it in this node
                child_ptr cp;
                cp.value = new_value;
                return insert_entry_checked(found_entry, cp, new_value_size);
            }
            else
            {
                // find where it should go
                int found_entry = search_node(insert_offset);
                
                // insert it in a subtree
                child_ptr insert_new_sub_node;
                insert_new_sub_node.subtree = p[found_entry].subtree->insert(insert_offset - offset[found_entry], new_value, new_value_size);
                
                if (insert_new_sub_node.subtree)
                {
                    // we have a new node from below which we need to insert here
                    return insert_entry_checked(found_entry+1, insert_new_sub_node, new_value_size);
                }
                else
                {
                    // nothing to insert above
                    return NULL;
                }
            }
        }

        T *remove(size_t delete_offset, size_t &deleted_size)
        {
            // find where we're removing it from
            int found_entry = search_node(delete_offset);
            if (is_leaf)
            {
                // delete from a leaf
                deleted_size = entry_size(found_entry);
                return delete_entry(found_entry);
            }
            else
            {
                // recursively delete from a branch
                cbnode<T, order> *found_subtree = p[found_entry].subtree;
                T *deleted_value = found_subtree->remove(delete_offset - offset[found_entry], deleted_size);
                
                if (found_subtree->num_entries < ((order+1)/2))
                {
                    // the entry we just deleted from now has too few items in it.
                    // try to redistribute items to keep it at least half full.
                    rebalance(found_entry);
                }
                
                // fix the total size
                total_size -= deleted_size;
                
                return deleted_value;
            }
        }
    };

    
    //
    // The root node of a counted B+-tree.
    // This may have 0, 1, or 2 children
    //
    // In this template:
    //  T is the data type of the values to store
    //  order is the maximum number of children in the B+-tree
    //
    
    template <class T, int order>
    class cbtree
    {
    private:
        cbnode<T, order> *root;
        
    public:
        cbtree()
        {
            root = new cbnode<T, order>(true);
        }
        
        ~cbtree()
        {
            delete root;
        }

        size_t size()
        {
            return root->size();
        }
            
        void insert(size_t insert_offset, T *new_value, size_t new_value_size)
        {
            cbnode<T, order> *new_sub_node = root->insert(insert_offset, new_value, new_value_size);
            
            if (new_sub_node)
            {
                // we have a new node from below. increase the height of the tree
                root = new cbnode<T, order>(root, new_sub_node);
            }
        }
        
        void append(T *new_value, size_t new_value_size)
        {
            insert(root->size(), new_value, new_value_size);
        }
        
        T *remove(size_t remove_offset)
        {
            size_t deleted_size;
            T *deleted_value = root->remove(remove_offset, deleted_size);
            
            if (root->get_num_entries() == 1)
            {
                // the root node is allowed to be underweight but it's possible
                // it may have only one entry, in which case we'll eliminate it
                // and make the tree one level less deep
                root = root->eliminate_root();
            }
            
            return deleted_value;
        }
        
        T *lookup(size_t pos, size_t &found_offset)
        {
            return root->lookup(pos, found_offset);
        }
        
        T *lookup(size_t pos)
        {
            size_t found_offset;
            return root->lookup(pos, found_offset);
        }
    };

}

#endif // DEEPC_CBTREE_H

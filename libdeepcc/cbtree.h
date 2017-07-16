//////////////////////////////////////////////////////////////////////////
///                                                                    ///
///                          DeepC compiler                            ///
///                                                                    ///
///                      Source file collection                        ///
///                                                                    ///
///                                   - Copyright 2017 Zik Saleeba     ///
///                                           2017-03-27               ///
///                                                                    ///
//////////////////////////////////////////////////////////////////////////

#ifndef DEEPCOMPILER_CBTREE_H_
#define DEEPCOMPILER_CBTREE_H_

//
// This module defines a "counted-only" B+-tree. This is an
// auto-balanced tree which is accessed like an array, using
// array-like indices to address data. Unlike an array however
// the structure can be easily expanded and contracted using
// insertion and deletion operations to any part of the array.
// These operations are are quick, taking O(log n) time compared
// to O(n) for a conventional array. On the other hand data is
// accessed with O(log n) time, which is slower than an array's
// constant access time. However the tree hierarchy is kept as 
// flat as possible so most accesses are close to O(1) most of
// the time.
//
// This variant on the counted B+-tree doesn't use keys at all,
// hence the "counted-only" name. Also, the values we store
// are buffers which each have more than one addressable element
// stored in them, so we keep the number of elements below each
// node in the node in addition to the usual B+-tree structure. This
// allows us to track this extra information.
//
// This implementation allows the order of the tree to be  
// adjusted for tuning purposes. The larger the order of the  
// tree the higher the performance will be, but also the greater 
// the amount of wasted space. Increasing the order makes the 
// tree shallower, but also makes the linear operations on
// individual nodes take longer so some balancing of these two
// factors is necessary for the best performance. Using higher
// orders also has a second effect of taking advantage of cache
// coherency for added performance.
//
// For a description of counted B-trees see here:
// http://www.chiark.greenend.org.uk/~sgtatham/algorithms/cbtree.html
//

#include <utility>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstddef>

namespace deepC {

// Forward declaration of the iterator type.
 
template <class T, int kOrder> class cbtree_iter;

//
// A generic node of a counted B+-tree
//
// Nodes can be either "leaf" or "branch" (internal nodes).
// This class handles both types of node.
//
// In this template:
//  T is the data type of the values to store
//  order is the maximum number of children in the B+-tree
//

template <class T, unsigned int kOrder>
class cbnode {
    // A pointer which can pointer to either a subtree or a value.
    // These are used to point down the tree.
    union child_ptr {
        cbnode<T, kOrder> *subtree_;  // Subtrees of a branch.
        T *value_;                    // Values in a leaf.
    };

    unsigned char      is_leaf_;          // True if this node is a leaf.
    uint32_t           num_entries_;      // The number of values or subtrees in this node.
    size_t             total_size_;       // Size of all the elements in this node and all sub-nodes.
    cbnode<T, kOrder> *prev_;             // The previous leaf in the linked list of leaf nodes.
    cbnode<T, kOrder> *next_;             // The next leaf in the linked list of leaf nodes.
    size_t             offset_[kOrder];   // The offsets of each value within the node.
    child_ptr          sub_[kOrder];      // Subtrees of a branch or values in a leaf.

    friend class cbtree_iter<T, kOrder>;

private:
    //
    // NAME:        getEntrySize
    // ACTION:      Get the size of a single entry in the node.
    //              This is made slightly more complicated due to us storing
    //              only the offsets but not the sizes of each entry.
    // PARAMETERS:  int entry - which entry in the node to check.
    // RETURNS:     size_t - the entry size.
    //

    size_t getEntrySize(uint32_t entry) const {
        if (entry == num_entries_ - 1) {
            return total_size_ - offset_[entry];
        } else {
            return offset_[entry + 1] - offset_[entry];
        }
    }

    //
    // NAME:        searchNode
    // ACTION:      Search the node for an offset.
    //              This may find the offset in a subtree or if it's a leaf
    //              it will find the value the offset refers to. Note that
    //              no out-of-bounds check is done - if the offset is out
    //              of bounds the last subtree will be returned.
    // PARAMETERS:  size_t find_offset - the index of the thing we're looking
    //                  for in the node.
    // RETURNS:     int - the index of the found entry.
    //

    int searchNode(size_t find_offset) const {
        // Binary search within the node.
        int left_entry = 0;
        int right_entry = num_entries_;

        while (right_entry - left_entry > 1) {
            int mid_entry = (left_entry + right_entry) >> 1;

            if (find_offset < offset_[mid_entry]) {
                right_entry = mid_entry;
            } else {
                left_entry = mid_entry;
            }
        }

        return left_entry;
    }

    //
    // NAME:        appendContents
    // ACTION:      Moves the contents of a given node across to the end of
    //              this node, appending to the current contents.
    //              Assumes that both will fit in the one node.
    // PARAMETERS:  cbnode<T, kOrder> *from_node - the node to move data from.
    //

    void appendContents(cbnode<T, kOrder> *from_node) {
        // Move the values across.
        std::move(&from_node->sub_[0], &from_node->sub_[from_node->num_entries_], &sub_[num_entries_]);

        // Move the offsets across and adjust.
        for (uint_fast32_t count = 0; count < from_node->num_entries_; count++) {
            offset_[num_entries_ + count] = from_node->offset_[count] + total_size_;
        }

        // Fix sizes.
        total_size_ += from_node->total_size_;
        num_entries_ += from_node->num_entries_;
        from_node->total_size_ = 0;
        from_node->num_entries_ = 0;
    }

    //
    // NAME:        splitNode
    // ACTION:      Splits a node in half to make space for insertion.
    // PARAMETERS:  bool insert_left - true to leave more space on the
    //                  left, otherwise leaves more space on the right.
    // RETURNS:     cbtree<T, kOrder> * - the new node which is to the
    //                  right of this node.
    //

    cbnode<T, kOrder> *splitNode(bool insert_left) {
        cbnode<T, kOrder> *right_node = new cbnode<T, kOrder>(is_leaf_);

        // Where will we split the node?
        int split_entry;

        if (insert_left) {
            // Move more data to the right.
            split_entry = num_entries_ / 2;
        } else {
            // Leave more data on the left.
            split_entry = num_entries_ - num_entries_ / 2;
        }

        // Adjust sizes and entries.
        right_node->num_entries_ = num_entries_ - split_entry;
        right_node->total_size_ = total_size_ - offset_[split_entry];
        num_entries_ = split_entry;
        total_size_ = offset_[split_entry];

        // Copy values/subtrees across.
        std::copy_n(&sub_[split_entry], right_node->num_entries_, &right_node->sub_[0]);
        std::fill_n(&sub_[split_entry].subtree_, kOrder - split_entry, nullptr);

        // Copy offsets across and adjust.
        for (uint_fast32_t count = 0; count < right_node->num_entries_; count++) {
            right_node->offset_[count] = offset_[count + split_entry] - total_size_;
        }

        std::fill_n(&offset_[split_entry], kOrder - split_entry, 0);

        // Are we a leaf?
        if (is_leaf_) {
            // Link the new node in.
            right_node->next_ = next_;
            right_node->prev_ = this;
            if (next_) {
                next_->prev_ = right_node;
            }

            next_ = right_node;
        }
        
        return right_node;
    }

    //
    // NAME:        insertValue
    // ACTION:      Insert a value into this leaf node. Assumes there's space in the node.
    // PARAMETERS:  unsigned int new_entry - which entry/bucket to insert it in this node.
    //              T *new_value           - the value to insert.
    //              size_t new_value_size  - the number of values which this item contains.
    //

    void insertValue(unsigned int new_entry, T *new_value, size_t new_value_size) {
        // There's room - find where we're going to insert it.
        if (new_entry >= num_entries_) {
            // Append it to the node.
            sub_[num_entries_].value_ = new_value;
            offset_[num_entries_] = total_size_;
        } else {
            // Insert into the node - copy all the values to the right across by one.
            std::copy_n(&sub_[new_entry], num_entries_ - new_entry, &sub_[new_entry + 1]);
            sub_[new_entry].value_ = new_value;

            // Copy all the offsets to the right and adjust them.
            for (uint_fast32_t count = num_entries_; count > new_entry; count--) {
                offset_[count] = offset_[count-1] + new_value_size;
            }
        }

        num_entries_++;
        total_size_ += new_value_size;
    }
    
    //
    // NAME:        insertSubtree
    // ACTION:      Insert a subtree into this branch node. Assumes there's space in the node.
    // PARAMETERS:  unsigned int new_entry     - which entry/bucket to insert it in this node.
    //              cbnode<T, kOrder> *subtree - the subtree to insert.
    //

    void insertSubtree(unsigned int new_entry, cbnode<T, kOrder> *subtree) {
        // There's room - find where we're going to insert it.
        if (new_entry >= num_entries_) {
            // Append it to the node.
            sub_[num_entries_].subtree_ = subtree;
            offset_[num_entries_] = total_size_;
        } else {
            // Insert into the node - copy all the values to the right across by one.
            std::copy_n(&sub_[new_entry], num_entries_ - new_entry, &sub_[new_entry + 1]);
            sub_[new_entry].subtree_ = subtree;

            // Copy all the offsets to the right and adjust them.
            for (uint_fast32_t count = num_entries_; count > new_entry; count--) {
                offset_[count] = offset_[count-1] + subtree->size();
            }
        }

        // Update the counters.
        num_entries_++;
        total_size_ += subtree->size();
    }

    //
    // NAME:        deleteEntry
    // ACTION:      Delete an entry from this node.
    // PARAMETERS:  unsigned int entry   - which entry/bucket to insert it in this node.
    //              size_t *deleted_size - set to the number of values the item contains.
    //

    child_ptr deleteEntry(unsigned int entry, size_t *deleted_size) {
        child_ptr deleted_value = sub_[entry];

        // How much are we deleting?
        *deleted_size = getEntrySize(entry);

        // Move the values across.
        std::memmove(&sub_[entry],
                     &sub_[entry + 1],
                     sizeof(sub_[entry]) * (num_entries_ - entry - 1));
        sub_[num_entries_ - 1].subtree_ = nullptr;

        // Move the offsets across and adjust them.
        for (uint_fast32_t count = entry; count < num_entries_ - 1; count++) {
            offset_[count] = offset_[count + 1] - *deleted_size;
        }
        
        offset_[num_entries_ - 1] = 0;

        total_size_ -= *deleted_size;
        num_entries_--;

        return deleted_value;
    }

    //
    // NAME:        rebalance
    // ACTION:      Rebalances the tree for optimal performance.
    //              Should only be called on branches which are underweight.
    // PARAMETERS:  unsigned int underweight_entry - the entry which is underweight.
    //

    void rebalance(unsigned int underweight_entry) {
        cbnode<T, kOrder> *underweight_subtree = sub_[underweight_entry].subtree_;

        // Is there an entry to the left of the underweight one?
        if (underweight_entry > 0) {
            // Yes, can we steal an item from the left?
            cbnode<T, kOrder> *left_subtree = sub_[underweight_entry - 1].subtree_;

            if (left_subtree->num_entries_ > ((kOrder + 1) / 2)) {
                // Yes, steal from the left
                size_t    stolen_size;
                child_ptr steal_value = left_subtree->deleteEntry(left_subtree->num_entries_ - 1, &stolen_size);
                if (underweight_subtree->is_leaf_) {
                    underweight_subtree->insertValue(0, steal_value.value_, stolen_size);
                }
                else {
                    underweight_subtree->insertSubtree(0, steal_value.subtree_);
                }
                offset_[underweight_entry] += stolen_size;
            } else {
                // No, but we can add the contents of this node to the node
                // on the left.
                left_subtree->appendContents(underweight_subtree);

                // Remove the underweight node.
                delete underweight_subtree;
                size_t deleted_size;
                deleteEntry(underweight_entry, &deleted_size);  // XXX - do we need to delete this node?
            }
        } else if (underweight_entry + 1 < num_entries_) {
            // Can we steal an item from the right instead?
            cbnode<T, kOrder> *right_subtree = sub_[underweight_entry + 1].subtree_;

            if (right_subtree->num_entries_ > ((kOrder + 1) / 2)) {
                // Yes, steal from the right.
                size_t    stolen_size;
                child_ptr steal_value = right_subtree->deleteEntry(0, &stolen_size);
                if (underweight_subtree->is_leaf_) {
                    underweight_subtree->insertValue(num_entries_, steal_value.value_, stolen_size);
                }
                else {
                    underweight_subtree->insertSubtree(num_entries_, steal_value.subtree_);
                }
                
                offset_[underweight_entry] += stolen_size;
            } else {
                // No, but we can add the contents of this node to the node
                // on the right.
                right_subtree->appendContents(underweight_subtree);

                // Remove the underweight node.
                delete underweight_subtree;
                size_t deleted_size;
                deleteEntry(underweight_entry, &deleted_size);  // XXX - do we need to delete this entry?
            }
        }
    }

  public:
    //cbnode() : num_entries_(0), is_leaf_(false), total_size_(0) {}
    explicit cbnode(bool p_is_leaf)
        : is_leaf_(p_is_leaf), num_entries_(0), total_size_(0), prev_(nullptr), next_(nullptr)
    {
        std::fill_n(&offset_[0], kOrder, 0);
        if (!p_is_leaf)
        {
            for (unsigned int i = 0; i < kOrder; i++) {
                sub_[i].value_ = nullptr;
            }
        }
        else
        {
            for (unsigned int i = 0; i < kOrder; i++) {
                sub_[i].subtree_ = nullptr;
            }
        }
    }

    // Accessors.
    size_t   size() const                { return total_size_; }
    bool     isLeaf() const              { return is_leaf_; }
    size_t   getNumEntries() const       { return num_entries_; }
    size_t   getOffset(size_t i) const   { return offset_[i]; }
    T *      getSubValue(size_t i) const { return sub_[i].value_; }
    cbnode<T, kOrder> *getSubTree(size_t i) const { return sub_[i].subtree_; }

    //
    // NAME:        Branch node constructor
    // ACTION:      Constructs a branch node from insert information passed
    //              up from a call to insert(). We need to create a new root
    //              node based on that information.
    // PARAMETERS:  cbnode<T, kOrder> *left, cbnode<T, kOrder> *right
    //                  - the information to put in the new node
    //

    cbnode(cbnode<T, kOrder> *left, cbnode<T, kOrder> *right)
        : is_leaf_(false), num_entries_(2), prev_(nullptr), next_(nullptr) {
        total_size_ = left->total_size_ + right->total_size_;
        sub_[0].subtree_ = left;
        sub_[1].subtree_ = right;
        offset_[0] = 0;
        offset_[1] = left->total_size_;
        
        for (unsigned int i = 2; i < kOrder; i++) {
            sub_[i].subtree_ = nullptr;
            offset_[i] = 0;
        }
    }

    ~cbnode() {
        // Delete all the subtrees.
        if (!is_leaf_) {
            for (uint_fast32_t count = 0; count < num_entries_; count++)
                delete sub_[count].subtree_;
        }
    }

//    //
//    // NAME:        linkInsertRight
//    // ACTION:      Insert a node to to the right of this node.
//    // PARAMETERS:  cbnode<T, kOrder> *new_node - the node to insert.
//    //

//    void linkInsertRight(cbnode<T, kOrder> *new_node) {
//        new_node->prev_ = this;
//        new_node->next_ = this->next_;
//        this->next_ = new_node;

//        if (new_node->next_ != nullptr)
//            new_node->next_->prev_ = new_node;
//    }

    //
    // NAME:        linkDelete
    // ACTION:      Delete this node from the list of leaves.
    //

    void linkDelete() {
        if (this->prev_ != nullptr) {
            this->prev_->next_ = this->next_;
        }

        if (this->next_ != nullptr) {
            this->next_->prev_ = this->prev_;
        }
    }

    //
    // NAME:        lookup
    // ACTION:      Look up an offset.
    //              Finds the value containing the given offset. Also
    //              returns the offset of the found object in the tree.
    // PARAMETERS:  size_t offset        - the offset to look for.
    //              T **found_item      - set to point to the found object.
    //              size_t *found_offset - set to the base offset of the found value.
    // RETURNS:     bool - true if found, false otherwise.
    //

    bool lookup(size_t lookup_offset, T **found_item, size_t *found_offset) {
        // Check for an out-of-bounds offset.
        if (lookup_offset >= total_size_)
            return false;

        // Search down the tree until we find a leaf.
        cbnode<T, kOrder> *search_subtree = this;
        size_t search_offset = 0;
        while (!search_subtree->is_leaf_) {
            int branch_entry = search_subtree->searchNode(lookup_offset - search_offset);
            search_offset += search_subtree->offset_[branch_entry];
            search_subtree = search_subtree->sub_[branch_entry].subtree_;
        }

        // Find the value we're looking for in the leaf.
        int leaf_entry = search_subtree->searchNode(lookup_offset - search_offset);
        *found_offset = search_offset + search_subtree->offset_[leaf_entry];
        *found_item = search_subtree->sub_[leaf_entry].value_;
        
        return true;
    }

    //
    // NAME:        insert
    // ACTION:      Insert a value into the tree.
    // PARAMETERS:  size_t insert_offset  - the offset into the node to insert at.
    //              T *new_value          - the value to insert.
    //              size_t new_value_size - the number of objects in the value.
    //              ssize_t adjust_parent_size - the number of items the parent should change by.
    // RETURNS:     cbtree<T, kOrder> *   - a new node we have to insert in the parent or 
    //                                      nullptr if none.
    //

    cbnode<T, kOrder> *insert(size_t insert_offset, T *new_value,
                             size_t new_value_size, ssize_t *adjust_parent_size) {
        cbnode<T, kOrder> *right_branch = nullptr;
        *adjust_parent_size = 0;
        
        if (is_leaf_) {
            // Find where it should go.
            unsigned int found_entry;
            if (insert_offset >= total_size_) {
                found_entry = num_entries_;  // it'll go right at the end
            } else {
                found_entry = searchNode(insert_offset);
            }

            // Insert it in this node.
            if (num_entries_ < kOrder) {
                // There's room - insert it.
                insertValue(found_entry, new_value, new_value_size);
                *adjust_parent_size += new_value_size;
            } else {
                // We have to split the node in two.
                bool insert_left = found_entry <= num_entries_ / 2;
                right_branch = splitNode(insert_left);
                *adjust_parent_size -= right_branch->size();

                if (insert_left) {
                    // Insert in the left node.
                    insertValue(found_entry, new_value, new_value_size);
                    *adjust_parent_size += new_value_size;
                } else {
                    // Insert in the right node.
                    right_branch->insertValue(found_entry - num_entries_, new_value, new_value_size);
                }
            }
        } else {
            // Find where it should go.
            int found_entry;
            if (insert_offset >= total_size_) {
                found_entry = num_entries_ - 1;  // It'll go in the last one.
            } else {
                found_entry = searchNode(insert_offset);
            }

            // Insert it in a subtree.
            ssize_t adjust_our_size = 0;
            cbnode<T, kOrder> *subtree =
                sub_[found_entry].subtree_->insert(
                    insert_offset - offset_[found_entry], new_value,
                    new_value_size, &adjust_our_size);
            
            // Adjust sizes and offsets.
            for (uint_fast32_t count = found_entry + 1; count < num_entries_; count++) {
                offset_[count] += adjust_our_size;
            }

            total_size_ += adjust_our_size;
            *adjust_parent_size += adjust_our_size;
            
            if (subtree) {
                // We have a new node from below which we need to insert here.
                unsigned int new_entry = found_entry + 1;
                if (num_entries_ < kOrder) {
                    // It'll fit in this node - insert it.
                    insertSubtree(new_entry, subtree);
                    *adjust_parent_size += subtree->size();
                }
                else {
                    // We have to split the node in two.
                    bool insert_left = new_entry <= num_entries_ / 2;
                    right_branch = splitNode(insert_left);
                    *adjust_parent_size -= right_branch->size();

                    if (insert_left) {
                        // Insert in the left node.
                        insertSubtree(new_entry, subtree);
                        *adjust_parent_size += subtree->size();
                    } else {
                        // Insert in the right node.
                        right_branch->insertSubtree(new_entry - num_entries_, subtree);
                    }
                }
            }
        }

        return right_branch;
    }

    //
    // NAME:        remove
    // ACTION:      Remove an item from the node.
    // PARAMETERS:  size_t delete_offset - the offset to delete at.
    //              size_t *deleted_size - set to the number of objects deleted.
    //

    T *remove(size_t delete_offset, size_t *deleted_size) {
        // Find where we're removing it from.
        int found_entry = searchNode(delete_offset);
        if (is_leaf_) {
            // Delete from a leaf.
            return deleteEntry(found_entry, deleted_size).value_;
        } else {
            // Recursively delete from a branch.
            cbnode<T, kOrder> *found_subtree = sub_[found_entry].subtree_;
            T *deleted_value = found_subtree->remove(
                delete_offset - offset_[found_entry], deleted_size);

            if (found_subtree->num_entries_ < ((kOrder + 1) / 2)) {
                // The entry we just deleted from now has too few items in it.
                // Try to redistribute items to keep it at least half full.
                rebalance(found_entry);
            }

            // Fix the total size.
            total_size_ -= *deleted_size;

            return deleted_value;
        }
    }
    
    //
    // NAME:        print
    // ACTION:      Prints out the tree. Used for testing.
    // RETURNS:     int - the start offset of this node.
    //              int - the depth.
    //

    void print(int offset, int depth) const {

        for (int indent = 0; indent < depth; indent++) {
            std::cout << "    ";
        }
        
        std::cout << "{ [" << offset << "-" << (offset + total_size_) << "], size=" << total_size_ << std::endl;
        
        if (is_leaf_) {
            for (uint_fast32_t i = 0; i < num_entries_; i++)
            {
                for (int indent = 0; indent < depth+1; indent++) {
                    std::cout << "    ";
                }
                
                std::cout << "[" << (offset + offset_[i]) << "]: " << (*sub_[i].value_).front() << "-" << (*sub_[i].value_).back() << std::endl;
            }
        }
        else {
            for (uint_fast32_t i = 0; i < num_entries_; i++)
            {
                sub_[i].subtree_->print(offset + offset_[i], depth+1);
            }
        }

        for (int indent = 0; indent < depth; indent++) {
            std::cout << "    ";
        }

        std::cout << "}" << std::endl;        
    }
};

//
// The root node of a counted B+-tree.
// This may have 0, 1, or 2 children
//
// In this template:
//  T is the data type of the values to store
//  kOrder is the maximum number of children in the B+-tree
//

template <class T, int kOrder>
class cbtree {
  public:
    // For STL and iterators.
    friend class cbtree_iter<T, kOrder>;
    typedef cbtree_iter<T, kOrder> iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef T value_type;
    typedef T * pointer;
    typedef T & reference;
    
  private:
    // The root node.
    cbnode<T, kOrder> *root_;

  public:
    // Create an empty cbtree.
    cbtree() { 
        root_ = new cbnode<T, kOrder>(true); 
    }

    // Destructor.
    ~cbtree() {
        delete root_; 
    }

    //
    // NAME:        size
    // ACTION:      Get the size of the cbtree.
    // RETURNS:     size_t - the number of items in the cbtree.
    //

    size_t size() { 
        return root_->size(); 
    }

    //
    // NAME:        insert
    // ACTION:      Insert a new entry into the cbtree. This entry is normally
    //              a data structure which can contain multiple items.
    // PARAMETERS:  size_t insert_offset - the "array index" to insert at - ie. the
    //                  item offset. It's assumed that this offset will fall on a
    //                  boundary of one of the entries.
    //              T *new_entry - the entry to insert.
    //              size_t new_value_size - the number of items in the inserted value.
    //

    void insert(size_t insert_offset, T *new_entry, size_t new_entry_size) {
        ssize_t adjust_my_size = 0;
        cbnode<T, kOrder> *new_sub_node = root_->insert(insert_offset, new_entry, new_entry_size, &adjust_my_size);

        if (new_sub_node) {
            // We have a new node from below. increase the height of the tree.
            root_ = new cbnode<T, kOrder>(root_, new_sub_node);
        }
    }

    //
    // NAME:        append
    // ACTION:      Append an entry to the cbtree. This entry is normally
    //              a data structure which can contain multiple items.
    // PARAMETERS:  T *new_entry - the new entry to append.
    //              size_t new_entry_size - the number of items in the appended entry.
    //

    void append(T *new_entry, size_t new_entry_size) {
        insert(root_->size(), new_entry, new_entry_size);
    }

    //
    // NAME:        remove
    // ACTION:      Removes an entry from the cbtree. This will normally be a
    //              whole container with multiple items.
    // PARAMETERS:  size_t remove_offset - the offset of the entry to remove.
    // RETURNS:     T * - the removed entry.
    //

    T *remove(size_t remove_offset) {
        size_t deleted_size;
        return root_->remove(remove_offset, &deleted_size);
    }

    //
    // NAME:        lookup
    // ACTION:      Finds the entry at a specific offset.
    // PARAMETERS:  size_t pos           - the offset to find.
    //              T **found_item       - the found object or nullptr if not found.
    //              size_t *found_offset - set to the actual offset at the start
    //                  of the found entry. Since entries are containers the pos
    //                  might be in the middle of the container so this value
    //                  could be before pos.
    // RETURNS:     bool - true if found, false otherwise.
    //

    bool lookup(size_t pos, T **found_item, size_t *found_offset) {
        return root_->lookup(pos, found_item, found_offset);
    }
    
    //
    // NAME:        begin
    // ACTION:      Returns an iterator to the first leaf node in the tree.
    // RETURNS:     iterator
    //
    
    iterator begin() const {
        // Find the leftmost leaf node.
        cbnode<T, kOrder> *node = root_;
        while (!node->isLeaf()) {
            node = node->getSubTree(0);
        }
        
        return iterator(node, 0);
    }
    
    //
    // NAME:        end
    // ACTION:      Returns an iterator to the end of the tree.
    // RETURNS:     iterator
    //
    
    iterator end() const {
        return iterator(nullptr, 0);
    }
    
    //
    // NAME:        print
    // ACTION:      Diagnostic print of the tree.
    //
    
    void print() const {
        root_->print(0, 0);
    }
    
    // Accessor for testing.
    cbnode<T, kOrder> *getRoot() const {
        return root_;
    }
};


//
// Iterator for cbtree.
//

template <class T, int kOrder>
class cbtree_iter
{
    cbnode<T, kOrder> *node_;
    uint_fast16_t      entry_;
    
public:
    // Default constructor. Same as end().
    cbtree_iter() :
        node_(nullptr),
        entry_(0)
    {
    }
    
    // Constructor for an arbitrary node.
    cbtree_iter(cbnode<T, kOrder> *node, uint_fast16_t entry) :
        node_(node),
        entry_(entry)
    {
    }
    
    // Equality operators.
    bool operator=(const cbtree_iter<T, kOrder> &x) const {
        return node_ == x.node_ && entry_ == x.entry_;
    }
    
    bool operator!=(const cbtree_iter<T, kOrder> &x) const {
        return node_ != x.node_ || entry_ != x.entry_;
    }
    
    // Dereference operator.
    T * operator*() const {
        return node_->getSubValue(entry_);
    }
    
    // Increment operator.
    cbtree_iter<T, kOrder> operator++() { 
        entry_++;
        if (entry_ >= node_->num_entries_) {
            node_ = node_->next_;
            entry_ = 0;
        }
        return *this; 
    }
    
    cbtree_iter<T, kOrder> operator++(int) {
      cbtree_iter<T, kOrder> clone(*this);
      entry_++;
      if (entry_ >= node_->num_entries_) {
          node_ = node_->next_;
          entry_ = 0;
      }
      return clone;
    }
};

}  // namespace deepC

#endif  // DEEPCOMPILER_CBTREE_H_

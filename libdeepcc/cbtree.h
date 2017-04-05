﻿//////////////////////////////////////////////////////////////////////////
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
// For a description of counted B-trees see here:
// http://www.chiark.greenend.org.uk/~sgtatham/algorithms/cbtree.html
//

#include <utility>

namespace deepC {

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

template <class T, int kOrder>
class cbnode {
    // A pointer which can pointer to either a subtree or a value.
    // These are used to point down the tree.
    union child_ptr {
        cbnode<T, kOrder> *subtree_;  // Subtrees of a branch.
        T *value_;                    // Values in a leaf.
    };

    int    num_entries_;        // The number of values or subtrees in this node.
    bool   is_leaf_;            // True if this node is a leaf.
    size_t total_size_;         // Size of all the elements in this node and all sub-nodes.
    size_t    offset_[kOrder];  // The offsets of each value within the node.
    child_ptr sub_[kOrder];     // Subtrees of a branch or values in a leaf.

    cbnode<T, kOrder> *prev_;  // The previous leaf in the linked list of leaf nodes.
    cbnode<T, kOrder> *next_;  // The next leaf in the linked list of leaf nodes.

  private:
    //
    // NAME:        getEntrySize
    // ACTION:      Get the size of a single entry in the node.
    //              This is made slightly more complicated due to us storing
    //              only the offsets but not the sizes of each entry.
    // PARAMETERS:  int entry - which entry in the node to check.
    // RETURNS:     size_t - the entry size.
    //

    size_t getEntrySize(int entry) const {
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
        for (int count = 0; count < from_node->num_entries_; count++) {
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
        memcpy(&right_node->sub_[0], &sub_[split_entry], sizeof(sub_[split_entry]) * right_node->num_entries_);

        // Copy offsets across and adjust.
        for (int count = 0; count < right_node->num_entries_; count++) {
            right_node->offset_[count] = offset_[count + split_entry] - total_size_;
        }

        // Link the new node in.
        if (is_leaf_) {
            right_node->next_ = next_;
            right_node->prev_ = this;
            next_ = right_node;

            return right_node;
        }
    }

    //
    // NAME:        insertEntry
    // ACTION:      Insert an entry into this node. Assumes there's space in the node.
    // PARAMETERS:  int new_entry         - which entry/bucket to insert it in this node.
    //              child_ptr new_value   - the value to insert.
    //              size_t new_value_size - the number of values which this item contains.
    //

    void insertEntry(int new_entry, child_ptr new_value, size_t new_value_size) {
        // There's room - find where we're going to insert it.
        if (new_entry >= num_entries_) {
            // Append it to the node.
            sub_[num_entries_] = new_value;
        } else {
            // Insert into the node - copy all the values to the right across by one.
            std::move(&sub_[new_entry], &sub_[num_entries_ - new_entry], &sub_[new_entry + 1]);
            sub_[new_entry] = new_value;

            // Copy all the offsets to the right and adjust them.
            for (int count = new_entry; count < num_entries_; count++) {
                offset_[count + 1] = offset_[count] + new_value_size;
            }
        }

        // set the new offset
        if (new_entry == 0) {
            offset_[new_entry] = 0;
        } else {
            offset_[new_entry] = offset_[new_entry - 1] + new_value_size;
        }

        num_entries_++;
        total_size_ += new_value_size;
    }

    //
    // NAME:        insertEntryChecked
    // ACTION:      Insert an entry into this node. Checks if there's space
    //              available and splits the node if necessary.
    // PARAMETERS:  int new_entry         - which entry/bucket to insert it in this node.
    //              child_ptr new_value   - the value to insert.
    //              size_t new_value_size - the number of values which this item contains.
    //

    cbnode<T, kOrder> *insertEntryChecked(int new_entry,
                                         child_ptr new_value,
                                         size_t new_value_size) {
        // Is there enough room to fit it in the node?
        if (num_entries_ < kOrder) {
            // There's room - insert it.
            insertEntry(new_entry, new_value, new_value_size);

            // There's nothing that we need inserted up in the parent.
            return NULL;
        } else {
            // We have to split the node in two.
            bool insert_left = new_entry <= num_entries_ / 2;
            cbnode<T, kOrder> *right_branch = splitNode(insert_left);

            if (insert_left) {
                // Insert in the left node.
                insertEntry(new_entry, new_value, new_value_size);
            } else {
                // Insert in the right node.
                right_branch->insertEntry(new_entry - num_entries_,
                                          new_value, new_value_size);
            }

            return right_branch;
        }
    }

    //
    // NAME:        deleteEntry
    // ACTION:      Delete an entry from this node.
    // PARAMETERS:  int entry            - which entry/bucket to insert it in this node.
    //              size_t *deleted_size - set to the number of values the item contains.
    //

    child_ptr deleteEntry(int entry, size_t *deleted_size) {
        child_ptr deleted_value = sub_[entry];

        // How much are we deleting?
        *deleted_size = getEntrySize(entry);

        // Move the values across.
        memmove(&sub_[entry],
                &sub_[entry + 1],
                sizeof(sub_[entry]) * (num_entries_ - entry - 1));

        // Move the offsets across and adjust them.
        for (int count = entry; count < num_entries_ - 1; count++) {
            offset_[count] = offset_[count + 1] - *deleted_size;
        }

        total_size_ -= *deleted_size;
        num_entries_--;

        return deleted_value;
    }

    //
    // NAME:        rebalance
    // ACTION:      Rebalances the tree for optimal performance.
    //              Should only be called on branches which are underweight.
    // PARAMETERS:  int underweight_entry - the entry which is underweight.
    //

    void rebalance(int underweight_entry) {
        cbnode<T, offset_> *underweight_subtree = sub_[underweight_entry].subtree_;

        // Is there an entry to the left of the underweight one?
        if (underweight_entry > 0) {
            // Yes, can we steal an item from the left?
            cbnode<T, offset_> *left_subtree = sub_[underweight_entry - 1].subtree_;

            if (left_subtree->num_entries_ > ((kOrder + 1) / 2)) {
                // Yes, steal from the left
                size_t    stolen_size;
                child_ptr steal_value = left_subtree->deleteEntry(
                    left_subtree->num_entries_ - 1, stolen_size);
                underweight_subtree->insertEntry(0, steal_value,
                                                 stolen_size);
                offset_[underweight_entry] += stolen_size;
            } else {
                // No, but we can add the contents of this node to the node
                // on the left.
                left_subtree->appendContents(underweight_subtree);

                // Remove the underweight node.
                delete underweight_subtree;
                deleteEntry(underweight_entry);
            }
        } else if (underweight_entry + 1 < num_entries_) {
            // Can we steal an item from the right instead?
            cbnode<T, offset_> *right_subtree = sub_[underweight_entry + 1].subtree_;

            if (right_subtree->num_entries_ > ((kOrder + 1) / 2)) {
                // Yes, steal from the right.
                size_t    stolen_size;
                child_ptr steal_value =
                    right_subtree->deleteEntry(0, stolen_size);
                underweight_subtree->insertEntry(num_entries_, steal_value,
                                                 stolen_size);
                offset_[underweight_entry] += stolen_size;
            } else {
                // No, but we can add the contents of this node to the node
                // on the right.
                right_subtree->appendContents(underweight_subtree);

                // Remove the underweight node.
                delete underweight_subtree;
                deleteEntry(underweight_entry);
            }
        }
    }

  public:
    cbnode() : num_entries_(0), is_leaf_(false), total_size_(0) {}
    explicit cbnode(bool p_is_leaf)
        : num_entries_(0), is_leaf_(p_is_leaf), total_size_(0) {}

    size_t size() { return total_size_; }

    //
    // NAME:        Branch node constructor
    // ACTION:      Constructs a branch node from insert information passed
    //              up from a call to insert(). We need to create a new root
    //              node based on that information.
    // PARAMETERS:  cbnode<T, kOrder> *left, cbnode<T, kOrder> *right
    //                  - the information to put in the new node
    //

    cbnode(cbnode<T, kOrder> *left, cbnode<T, kOrder> *right)
        : num_entries_(2), is_leaf_(false) {
        total_size_ = left->total_size_ + right->total_size_;
        sub_[0].subtree_ = left;
        sub_[1].subtree_ = right;
        offset_[0] = 0;
        offset_[1] = left->total_size_;
    }

    ~cbnode() {
        // Delete all the subtrees.
        if (!is_leaf_) {
            for (int count = 0; count < num_entries_; count++)
                delete sub_[count].subtree_;
        }
    }

    //
    // NAME:        linkInsertRight
    // ACTION:      Insert a node to to the right of this node.
    // PARAMETERS:  cbnode<T, kOrder> *new_node - the node to insert.
    //

    void linkInsertRight(cbnode<T, kOrder> *new_node) {
        new_node->prev_ = this;
        new_node->next_ = this->next_;
        this->next_ = new_node;

        if (new_node->next_ != NULL)
            new_node->next_->prev_ = new_node;
    }

    //
    // NAME:        linkDelete
    // ACTION:      Delete this node from the list of leaves.
    //

    void linkDelete() {
        if (this->prev_ != NULL) {
            this->prev_->next_ = this->next_;
        }

        if (this->next_ != NULL) {
            this->next_->prev_ = this->prev_;
        }
    }

    //
    // NAME:        lookup
    // ACTION:      Look up an offset.
    //              Finds the value containing the given offset. Also
    //              returns the offset of the found object in the tree.
    // PARAMETERS:  size_t offset        - the offset to look for.
    //              size_t *found_offset - set to the base offset of the found value.
    // RETURNS:     T * - the found value or NULL if not found.
    //

    T *lookup(size_t lookup_offset, size_t *found_offset) {
        // Check for an out-of-bounds offset.
        if (lookup_offset >= total_size_)
            return NULL;

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
        return search_subtree->sub_[leaf_entry].value_;
    }

    //
    // NAME:        insert
    // ACTION:      Insert a value into the tree.
    // PARAMETERS:  size_t insert_offset  - the offset into the node to insert at.
    //              T *new_value          - the value to insert.
    //              size_t new_value_size - the number of objects in the value.
    //

    cbnode<T, kOrder> *insert(size_t insert_offset, T *new_value,
                             size_t new_value_size) {
        if (is_leaf_) {
            // Find where it should go.
            int found_entry;
            if (insert_offset >= total_size_)
                found_entry = num_entries_;  // it'll go right at the end
            else
                found_entry = searchNode(insert_offset);

            // Insert it in this node.
            child_ptr cp;
            cp.value_ = new_value;
            return insertEntryChecked(found_entry, cp, new_value_size);
        } else {
            // Find where it should go.
            int found_entry = searchNode(insert_offset);

            // Insert it in a subtree.
            child_ptr insert_new_sub_node;
            insert_new_sub_node.subtree_ =
                sub_[found_entry].subtree_->insert(
                    insert_offset - offset_[found_entry], new_value,
                    new_value_size);

            if (insert_new_sub_node.subtree_) {
                // We have a new node from below which we need to insert here.
                return insertEntryChecked(found_entry + 1, insert_new_sub_node, new_value_size);
            } else {
                // Nothing to insert above.
                return NULL;
            }
        }
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
            *deleted_size = getEntrySize(found_entry);
            return deleteEntry(found_entry);
        } else {
            // Recursively delete from a branch.
            cbnode<T, kOrder> *found_subtree = sub_[found_entry].subtree_;
            T *deleted_value = found_subtree->remove(
                delete_offset - offset_[found_entry], *deleted_size);

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
  private:
    cbnode<T, kOrder> *root_;

  public:
    // Create an empty cbtree.
    cbtree() { root_ = new cbnode<T, kOrder>(true); }

    // Destructor.
    ~cbtree() { delete root_; }

    //
    // NAME:        size
    // ACTION:      Get the size of the cbtree.
    // RETURNS:     size_t - the number of items in the cbtree.
    //

    size_t size() { return root_->size(); }

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
        cbnode<T, kOrder> *new_sub_node = root_->insert(insert_offset, new_entry, new_entry_size);

        if (new_sub_node) {
            // we have a new node from below. increase the height of the
            // tree
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
        T *    deleted_value = root_->remove(remove_offset, deleted_size);

        if (root_->get_num_entries() == 1) {
            // The root node is allowed to be underweight but it's possible
            // it may have only one entry, in which case we'll eliminate it
            // and make the tree one level less deep.
            root_ = root_->eliminate_root();
        }

        return deleted_value;
    }

    //
    // NAME:        lookup
    // ACTION:      Finds the entry at a specific offset.
    // PARAMETERS:  size_t pos - the offset to find.
    //              size_t *found_offset - set to the actual offset at the start
    //                  of the found entry. Since entries are containers the pos
    //                  might be in the middle of the container so this value
    //                  could be before pos.
    // RETURNS:     T * - the found object or nullptr if not found.
    //

    T *lookup(size_t pos, size_t *found_offset) {
        return root_->lookup(pos, found_offset);
    }

    //
    // NAME:        lookup
    // ACTION:      Finds the entry at a specific offset.
    // PARAMETERS:  size_t pos - the offset to find.
    // RETURNS:     T * - the found object or nullptr if not found.
    //

    T *lookup(size_t pos) {
        size_t found_offset;
        return root_->lookup(pos, &found_offset);
    }
};

}  // namespace DeepC

#endif  // DEEPCOMPILER_CBTREE_H_
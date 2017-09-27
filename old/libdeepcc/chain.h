/////////////////////////////////////////////////////////////////////////
////                                                                 ////
////                     "chain" data structure.                     ////
////                                                                 ////
////                           - by Zik Saleeba zik@zikzak.net       ////
////                                       2016-08-23                ////
////                                                                 ////
/////////////////////////////////////////////////////////////////////////

#ifndef DEEPC_CHAIN_H
#define DEEPC_CHAIN_H

#include <memory>
#include <vector>
#include <sys/types.h>


namespace deepC
{

//
// A generic node with reference counting and sub-item counting.
//

template <class T>
class ChainNode
{
protected:
    unsigned refCount_;
    size_t   itemCount_;

public:
    ChainNode() : refCount_(0), itemCount_(0) {}
    virtual ~ChainNode() {}

    void addRef() { refCount_++; }
    static void unRef(ChainNode<T> *node)
    {
        node->refCount--;
        if (node->refCount_ == 0)
            delete node;
    }

    size_t size() const { return itemCount_; }

    virtual T       &at(size_t i) = 0;
    virtual const T &at(size_t i) const = 0;

    virtual void append(const T &obj) = 0;
    virtual void insert(off_t off, const T &obj) = 0;
    virtual void remove(off_t off, size_t len) = 0;
};


//
// A payload node that contains a vector of items.
//

template <class T>
class ChainNodeVector : public ChainNode<T>
{
    std::vector<T> vec_;

public:
    ChainNodeVector() {}
    ~ChainNodeVector() {}

    T       &at(size_t i) { return vec_[i]; }
    const T &at(size_t i) const { return vec_[i]; }

    void append(const T &obj)
    {
        vec_.push_back(obj);
        itemCount_++;
    }

    void insert(off_t off, const T &obj)
    {
        vec_.insert(vec_.begin() + off, obj);
        itemCount_++;
    }

    void remove(off_t off, size_t len)
    {
        vec_.erase(vec_.begin() + off, vec_.begin() + off + len);
        itemCount_ -= len;
    }
};


//
// An intermediate node which says to use a sub-segment of the
// items beneath it. This is used when we want to use only part
// of a vector via a slice() operation for instance.
//

template <class T>
class ChainNodeSubset : public ChainNode<T>
{
    ChainNode<T> *sub_;
    off_t  start_;
    size_t len_;

public:
    ChainNodeSubset(ChainNode<T> *sub, off_t start, size_t len) : sub_(sub), start_(start), len_(len) {}
    ~ChainNodeSubset() { unRef(sub_); }

    T       &at(size_t off) { return sub_->at(start_ + off); }
    const T &at(size_t off) const { return sub_->at(start_ + off); }

    void append(const T &obj)
    {
        if (start_ + len_ == sub->size())
            sub_->append(obj);
        else
            sub_->insert(start_ + len_, obj);

        itemCount_++;
    }

    void insert(off_t off, const T &obj)
    {
        sub_->insert(start_ + off, obj);
        itemCount_++;
    }

    void remove(off_t off, size_t len)
    {
        sub_->remove(off + start_, len);
        itemCount_ -= len_;
    }
};


//
// A tree node. This is used when inserting or deleting to
// make a self-balacing tree of edits attached to a part of the
// chain.
//

template <class T>
class ChainNodeTree : public ChainNode<T>
{
    ChainNode<T> *left_;
    ChainNode<T> *right_;

public:
    ChainNodeTree(ChainNode<T> *left, ChainNode<T> *right) : left_(left), right_(right) {}
    ~ChainNodeTree()
    {
        if (left_ != nullptr)
        {
            unRef(left_);
        }

        if (right_ != nullptr)
        {
            unRef(right_);
        }
    }

    T &at(size_t i)
    {
        if (i < left_->size())
            return left_->at(i);
        else
            return right_->at(i - left_->size());
    }

    const T &at(size_t i) const
    {
        if (i < left_->size())
            return left_->at(i);
        else
            return right_->at(i - left_->size());
    }

    void append(const T &obj)
    {
        right_->push_back(obj);
        itemCount_++;
    }

    void insert(off_t off, const T &obj)
    {
        if (off > left_->size())
        {
            left_->insert(off, obj);
        }
        else
        {
            right_->insert(off - left_->size());
        }

        itemCount_++;
    }

    void remove(off_t off, size_t len)
    {
        if (off >= left_->size())
        {
            // Portion to remove is all on the right side.
            right_->remove(off - left_->size(), len);
        }
        else if (off + len <= left_->size())
        {
            // It's all on the left side.
            left_->remove(off, len);
        }
        else
        {
            // It's split over both sides.
            left_->remove(off, left_->size() - off);
            right_->remove(0, len + off - left_->size());
        }

        itemCount_ -= len;
    }
};


//
// The user-accessible top level part of a chain.
//

template <class T>
class Chain
{
    ChainNode<T> *root_;

public:
    Chain() : root_(new ChainNodeVector<T>) {}
    Chain(const T *data, size_t len) : root_(new ChainNodeVector<T>(data, len)) {}
    ~Chain() { delete root_; }

    T       &at(size_t i)       { return root_->at(i); }
    const T &at(size_t i) const { return root_->at(i); }

    void append(const T &obj)
    {
        root_->append(obj);
    }

    void insert(off_t off, const T &obj)
    {
        root_->insert(off, obj);
    }

    void remove(off_t off, size_t len)
    {
        root_->remove(off, len);
    }
};

}  // namespace deepC

#endif // DEEPC_CHAIN_H

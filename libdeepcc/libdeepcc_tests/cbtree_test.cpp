#include <iostream>
#include <vector>
#include <assert.h>
#include <gtest/gtest.h>

#include "cbtree.h"

namespace deepC
{

std::vector<int> *makeVec(int start, int len)
{
    std::vector<int> *vec = new std::vector<int>();
    vec->resize(len);
    for (int i = 0; i < len; i++)
    {
        (*vec)[i] = i + start;
    }

    return vec;
}


size_t consistency_check(const cbnode< std::vector<int>, 6 > *t) {
    if (t->isLeaf()) {
        return t->size();
    } 

    size_t total = 0;
    for (size_t i = 0; i < t->getNumEntries(); i++) {
        EXPECT_EQ(total, t->getOffset(i));
        total += consistency_check(t->getSubnode(i));
    }
    
    EXPECT_EQ(total, t->size());
    
    return total;
}


TEST(CBTree, Append)
{
    cbtree<std::vector<int>, 6> cb;

    auto v1 = makeVec(0, 100);
    cb.append(v1, v1->size());

    EXPECT_EQ(cb.size(), 100);

    for (size_t i = 0; i < cb.size(); i++)
    {
        std::vector<int> *vec;
        size_t offset;
        EXPECT_TRUE(cb.lookup(i, &vec, &offset));
        EXPECT_EQ(vec, v1);
        EXPECT_EQ(offset, 0);
    }

    delete v1;
}

TEST(CBTree, Append2)
{
    cbtree<std::vector<int>, 6> cb;

    auto v1 = makeVec(0, 100);
    cb.append(v1, v1->size());

    auto v2 = makeVec(100, 100);
    cb.append(v2, v2->size());

    EXPECT_EQ(cb.size(), 200);

    for (size_t i = 0; i < cb.size(); i++)
    {
        std::vector<int> *vec;
        size_t offset;
        EXPECT_TRUE(cb.lookup(i, &vec, &offset));
        if (i < 100)
        {
            EXPECT_EQ(vec, v1);
            EXPECT_EQ(offset, 0);
        }
        else
        {
            EXPECT_EQ(vec, v2);
            EXPECT_EQ(offset, 100);
        }
    }

    delete v1;
    delete v2;
}

TEST(CBTree, Insert1)
{
    cbtree<std::vector<int>, 6> cb;

    auto v1 = makeVec(0, 100);
    cb.append(v1, v1->size());

    auto v3 = makeVec(300, 100);
    cb.append(v3, v3->size());

    auto v2 = makeVec(100, 100);
    cb.insert(100, v2, v2->size());

    EXPECT_EQ(cb.size(), 300);

    for (size_t i = 0; i < cb.size(); i++)
    {
        std::vector<int> *vec;
        size_t offset;
        EXPECT_TRUE(cb.lookup(i, &vec, &offset));
        if (i < 100)
        {
            EXPECT_EQ(vec, v1);
            EXPECT_EQ(offset, 0);
        }
        else if (i < 200)
        {
            EXPECT_EQ(vec, v2);
            EXPECT_EQ(offset, 100);
        }
        else
        {
            EXPECT_EQ(vec, v3);
            EXPECT_EQ(offset, 200);
        }
    }

    delete v1;
    delete v2;
    delete v3;
}

TEST(CBTree, InsertN)
{
    cbtree<std::vector<int>, 6> cb;
    typedef std::vector<int> *vecintr_ptr;
    vecintr_ptr vecs[64];
    for (size_t i = 0; i < 64; i++)
    {
        vecs[i] = makeVec(i * 100, 100);
    }
    
    for (size_t i = 0; i < 64; i+=2)
    {
        cb.append(vecs[i], vecs[i]->size());
//        std::cout << "append " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
//        cb.print();
//        std::cout << std::endl;
        ASSERT_EQ(cb.size(), i * 100 / 2 + 100);

        consistency_check(cb.getRoot());
    }

    for (size_t i = 1; i < 64; i+=2)
    {
        cb.insert(i * 100, vecs[i], vecs[i]->size());
//        std::cout << "insert " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
//        cb.print();
//        std::cout << std::endl;
        ASSERT_EQ(cb.size(), 32 * 100 + i * 100 / 2 + 50);

        consistency_check(cb.getRoot());
    }

    EXPECT_EQ(cb.size(), 64 * 100);
    
//    cb.print();
    
    EXPECT_EQ(cb.min_depth(), 3);
    EXPECT_EQ(cb.max_depth(), 3);

    for (size_t i = 0; i < cb.size(); i++)
    {
        vecintr_ptr vec;
        size_t offset;
        ASSERT_TRUE(cb.lookup(i, &vec, &offset));
        ASSERT_EQ(vec->at(i - offset), i);
    }
    
    consistency_check(cb.getRoot());

    for (size_t i = 0; i < 64; i++)
    {
        delete vecs[i];
    }
}


TEST(CBTree, InsertRandom)
{
    cbtree< std::vector<int>, 4> cb;
    std::vector<int> cmp;
    
    srandom(42);
    
    for (int pass = 0; pass < 10000; pass++) {
        // Insert in the cbtree.
        size_t insertAt = random() % (pass + 1);
        std::vector<int> *insertVal = new std::vector<int>{pass};
//        std::cout << "insert " << pass << " at " << insertAt << " size=" << insertVal->size() << std::endl;
        
        cb.insert(insertAt, insertVal, insertVal->size());
        
//        cb.print();
//        std::cout << std::endl;

        // Do the same to the vector.
        cmp.resize(pass + 1);
        for (size_t i = pass; i > insertAt; i--)
        {
            cmp[i] = cmp[i-1];
        }

        cmp[insertAt] = pass;
    }
    
//    cb.print();
    
//    std::cout << "vector: ";
//    for (size_t i = 0; i < cmp.size(); i++)
//    {
//        std::cout << i << ":" << cmp[i];
//        if (i == cmp.size()-1)
//            std::cout << std::endl;
//        else
//            std::cout << " ";
//    }

//    std::cout << "cbtree: ";
//    for (size_t i = 0; i < cmp.size(); i++)
//    {
//        std::vector<int> *foundItem = nullptr;
//        size_t foundOffset = 0;
//        ASSERT_TRUE(cb.lookup(i, &foundItem, &foundOffset));

//        std::cout << foundOffset << ":" << (*foundItem)[0];
//        if (i == cmp.size()-1)
//            std::cout << std::endl;
//        else
//            std::cout << " ";
//    }
    
    std::vector<int> *foundItem = nullptr;
    size_t foundOffset = 0;
    cb.lookup(7, &foundItem, &foundOffset);    

    // Now compare the two.
    ASSERT_EQ(cb.size(), cmp.size());
    for (size_t i = 0; i < cb.size(); i++)
    {
        std::vector<int> *foundItem = nullptr;
        size_t foundOffset = 0;
        ASSERT_TRUE(cb.lookup(i, &foundItem, &foundOffset));
        ASSERT_EQ(foundItem->size(), 1);
        EXPECT_EQ((*foundItem)[0], cmp[i]);
        EXPECT_EQ(foundOffset, i);
    }

    //cb.print();
}

}  // namespace deepC.


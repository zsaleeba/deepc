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



TEST(CBTreeTest, Append)
{
    cbtree<std::vector<int>, 4> cb;

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

TEST(CBTreeTest, Append2)
{
    cbtree<std::vector<int>, 4> cb;

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

TEST(CBTreeTest, Insert1)
{
    cbtree<std::vector<int>, 4> cb;

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

TEST(CBTreeTest, InsertN)
{
    cbtree<std::vector<int>, 4> cb;
    typedef std::vector<int> *vecintr_ptr;
    vecintr_ptr vecs[64];
    for (size_t i = 0; i < 64; i++)
    {
        vecs[i] = makeVec(i * 100, 100);
    }
    
    for (size_t i = 0; i < 64; i+=2)
    {
        cb.append(vecs[i], vecs[i]->size());
        std::cout << "append " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
        cb.print();
        std::cout << std::endl;
        ASSERT_EQ(cb.size(), i * 100 / 2 + 100);
    }

    for (size_t i = 1; i < 64; i+=2)
    {
        cb.insert(i * 100, vecs[i], vecs[i]->size());
        std::cout << "insert " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
        cb.print();
        std::cout << std::endl;
        ASSERT_EQ(cb.size(), 32 * 100 + i * 100 / 2 + 50);
    }

    EXPECT_EQ(cb.size(), 64 * 100);
    
    cb.print();
    
    EXPECT_EQ(cb.min_depth(), 4);
    EXPECT_EQ(cb.max_depth(), 4);

    for (size_t i = 0; i < cb.size(); i++)
    {
        vecintr_ptr vec;
        size_t offset;
        ASSERT_TRUE(cb.lookup(i, &vec, &offset));
        ASSERT_EQ(vec->at(i - offset), i);
    }

    for (size_t i = 0; i < 64; i++)
    {
        delete vecs[i];
    }
}

}  // namespace deepC.


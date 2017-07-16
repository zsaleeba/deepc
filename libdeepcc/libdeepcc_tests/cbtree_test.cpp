#include <iostream>
#include <vector>
#include <assert.h>
#include <gtest/gtest.h>


#include "cbtree.h"

namespace deepC
{

constexpr unsigned int order = 30;
typedef std::vector<int> *vecintr_ptr;


class CBTest : public ::testing::Test
{
public:

    cbtree< std::vector<int>, order> cb;
    std::vector<int> cmp;

public:
    CBTest() {}
    void SetUp();
    void TearDown();
    
    void checkEqual();
};

void CBTest::SetUp()
{
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
}

void CBTest::TearDown()
{
    for (auto iter = cb.begin(); iter != cb.end(); iter++) {
        delete *iter;
    }
}

void CBTest::checkEqual()
{
    ASSERT_EQ(cb.size(), cmp.size());

    for (size_t i = 0; i < cb.size(); i++)
    {
        vecintr_ptr vec;
        size_t offset;
        ASSERT_TRUE(cb.lookup(i, &vec, &offset));
        ASSERT_EQ(vec->at(i - offset), cmp[i]);
    }
}


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


size_t consistency_check(const cbnode< std::vector<int>, order> *t) {
    if (t->isLeaf()) {
        return t->size();
    } 

    size_t total = 0;
    for (size_t i = 0; i < t->getNumEntries(); i++) {
        EXPECT_EQ(total, t->getOffset(i));
        total += consistency_check(t->getSubTree(i));
    }
    
    EXPECT_EQ(total, t->size());
    
    return total;
}


TEST(CBTree, Append)
{
    cbtree<std::vector<int>, order> cb;

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
    cbtree<std::vector<int>, order> cb;

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
    cbtree<std::vector<int>, order> cb;

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
    cbtree<std::vector<int>, order> cb;
    vecintr_ptr vecs[5000];
    for (size_t i = 0; i < 5000; i++)
    {
        vecs[i] = makeVec(i * 100, 100);
    }
    
    for (size_t i = 0; i < 5000; i+=2)
    {
        cb.append(vecs[i], vecs[i]->size());
//        std::cout << "append " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
//        cb.print();
//        std::cout << std::endl;
        ASSERT_EQ(cb.size(), i * 100 / 2 + 100);

        consistency_check(cb.getRoot());
    }

    for (size_t i = 1; i < 5000; i+=2)
    {
        cb.insert(i * 100, vecs[i], vecs[i]->size());
//        std::cout << "insert " << (*vecs[i])[0] << "-" << ((*vecs[i])[0] + vecs[i]->size()) << std::endl;
//        cb.print();
//        std::cout << std::endl;
        ASSERT_EQ(cb.size(), 5000 / 2 * 100 + i * 100 / 2 + 50);

        consistency_check(cb.getRoot());
    }

    EXPECT_EQ(cb.size(), 5000 * 100);
    
//    cb.print();
    
    for (size_t i = 0; i < cb.size(); i++)
    {
        vecintr_ptr vec;
        size_t offset;
        ASSERT_TRUE(cb.lookup(i, &vec, &offset));
        ASSERT_EQ(vec->at(i - offset), i);
    }
    
    consistency_check(cb.getRoot());

    for (size_t i = 0; i < 5000; i++)
    {
        delete vecs[i];
    }
}


TEST_F(CBTest, InsertRandom)
{
        
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


TEST(CBTree, InsertSpeed)
{
    cbtree< std::vector<int>, order> cb;
    uint32_t randval = 654321;
    size_t totalSize = 0;
    std::vector<int> *insertVal = new std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    
    for (int pass = 0; pass < 1000000; pass++) {
        // Insert in the cbtree.
        randval = 1103515245 * randval + 12345;
        uint32_t numInserted = randval & 0xf;
        randval = 1103515245 * randval + 12345;
        uint32_t insertAt;
        if (cb.size() > 0) {
            insertAt = randval % cb.size();
        }
        else {
            insertAt = 0;
        }
        
        cb.insert(insertAt, insertVal, numInserted);
        totalSize += numInserted;
    }
    
    EXPECT_EQ(totalSize, cb.size());
}


TEST_F(CBTest, Iterator)
{
//    std::cout << "iter: ";
    size_t i = 0;
    for (auto iter = cb.begin(); iter != cb.end() && i < cmp.size(); iter++, i++) {
//        std::cout << i << ":" << (**iter)[0] << "/" << cmp[i] << " ";
        ASSERT_EQ((**iter)[0], cmp[i]);
    }

//    std::cout << std::endl;
}


TEST_F(CBTest, LookupRandom)
{
    uint32_t randval = 654321;
    for (int pass = 0; pass < 10000000; pass++) {
        randval = 1103515245 * randval + 12345;
        size_t pos = randval % cb.size();
        std::vector<int> *foundItem = nullptr;
        size_t foundOffset = 0;
        ASSERT_TRUE(cb.lookup(pos, &foundItem, &foundOffset));
    }
}


TEST_F(CBTest, LookupVector)
{
    int total = 0;
    uint32_t randval = 654321;
    for (int pass = 0; pass < 10000000; pass++) {
        randval = 1103515245 * randval + 12345;
        size_t pos = randval % cmp.size();
        total += cmp.at(pos);
    }
    
    ASSERT_NE(total, 0);
}


TEST_F(CBTest, RemoveFirst)
{
    cb.remove(0);
    cmp.erase(cmp.begin());
    
    checkEqual();
}


TEST_F(CBTest, RemoveMiddle)
{
    size_t middle = cb.size() / 2;
    cb.remove(middle);
    cmp.erase(cmp.begin() + middle);
    
    checkEqual();
}


TEST_F(CBTest, RemoveLast)
{
    cb.remove(cb.size()-1);
    cmp.pop_back();
    
    checkEqual();
}

}  // namespace deepC.


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

#if 0
    for (size_t i = 0; i < cb.size(); i++)
    {
        EXPECT_EQ(cb[i], i);
    }
#endif

    delete v1;
}

}  // namespace deepC.


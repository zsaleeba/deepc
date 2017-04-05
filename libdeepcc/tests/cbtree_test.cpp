#include <iostream>
#include <vector>
#include <assert.h>

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

int main()
{
    std::cout << "cbtree\n";

    cbtree<std::vector<int>, 4> cb;

    auto v1 = makeVec(0, 100);
    cb.append(v1, v1->size());

    assert(cb->size() == 100);

    for (int i = 0; i < 100; i++)
    {
        assert(cb[i] == i);
    }

    delete v1;

    return 0;
}

}  // namespace deepC.


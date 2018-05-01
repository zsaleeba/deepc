#include <iostream>
#include <gtest/gtest.h>

#include "sourcefile.h"

#if 0
namespace deepC
{

class SourceFileTest : public ::testing::Test
{
public:
    SourceFile sf_;

public:
    SourceFileTest() {}
    void SetUp();
    void TearDown();
};


void SourceFileTest::SetUp()
{

}


void SourceFileTest::TearDown()
{

}

}
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

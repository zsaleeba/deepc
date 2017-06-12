#include <iostream>
#include <gtest/gtest.h>


int main(int argc, char **argv)
{
    std::cout << "diff test\n";
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

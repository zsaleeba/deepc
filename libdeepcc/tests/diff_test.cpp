#include <iostream>
#include <vector>
#include <assert.h>
#include <gtest/gtest.h>

#include "diff.h"

namespace deepC
{

TEST(DiffTest, Insert)
{
    std::string a = "hello\nthere\nare\nyou?\n";
    std::string b = "hello\nthere\nhow\nare\nyou?\n";

    Diff d(a, b);
    std::vector<DiffEdit> edits;
    d.diff(&edits);
    ASSERT_EQ(edits.size(), 1);
    EXPECT_EQ(edits[0].getOp(), DiffEdit::Op::INSERT);
    EXPECT_EQ(edits[0].getOffset(), 2);
    EXPECT_EQ(edits[0].getLength(), 1);
}

TEST(DiffTest, Delete)
{
    std::string a = "hello\nthere\nhow\nare\nyou?\n";
    std::string b = "hello\nthere\nare\nyou?\n";

    Diff d(a, b);
    std::vector<DiffEdit> edits;
    d.diff(&edits);
    ASSERT_EQ(edits.size(), 1);
    EXPECT_EQ(edits[0].getOp(), DiffEdit::Op::DELETE);
    EXPECT_EQ(edits[0].getOffset(), 2);
    EXPECT_EQ(edits[0].getLength(), 1);
}

TEST(DiffTest, Modify)
{
    std::string a = "hello\nthere\nhow\nare\nyou?\n";
    std::string b = "hello\nthere\nwho\nare\nyou?\n";

    Diff d(a, b);
    std::vector<DiffEdit> edits;
    d.diff(&edits);
    ASSERT_EQ(edits.size(), 2);
    EXPECT_EQ(edits[0].getOp(), DiffEdit::Op::DELETE);
    EXPECT_EQ(edits[0].getOffset(), 2);
    EXPECT_EQ(edits[0].getLength(), 1);
    EXPECT_EQ(edits[1].getOp(), DiffEdit::Op::INSERT);
    EXPECT_EQ(edits[1].getOffset(), 2);
    EXPECT_EQ(edits[1].getLength(), 1);
}

TEST(DiffTest, MultiPart)
{
    std::string a = "hello\nthere\nhow\nare\nyou?\nI\nam\na\nturnip.\n";
    std::string b = "hello\nthere\nwho\nare\nyou?\nI\nam\nthe\nturnip.\n";

    Diff d(a, b);
    std::vector<DiffEdit> edits;
    d.diff(&edits);
    ASSERT_EQ(edits.size(), 4);
    EXPECT_EQ(edits[0].getOp(), DiffEdit::Op::DELETE);
    EXPECT_EQ(edits[0].getOffset(), 2);
    EXPECT_EQ(edits[0].getLength(), 1);
    EXPECT_EQ(edits[1].getOp(), DiffEdit::Op::INSERT);
    EXPECT_EQ(edits[1].getOffset(), 2);
    EXPECT_EQ(edits[1].getLength(), 1);
    EXPECT_EQ(edits[2].getOp(), DiffEdit::Op::DELETE);
    EXPECT_EQ(edits[2].getOffset(), 7);
    EXPECT_EQ(edits[2].getLength(), 1);
    EXPECT_EQ(edits[3].getOp(), DiffEdit::Op::INSERT);
    EXPECT_EQ(edits[3].getOffset(), 7);
    EXPECT_EQ(edits[3].getLength(), 1);
}

}  // namespace deepC.


int main(int argc, char **argv)
{
    std::cout << "diff test\n";
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

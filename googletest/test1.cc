// https://github.com/google/googletest/blob/master/googletest/docs/primer.md
// https://github.com/google/googletest/blob/master/googletest/docs/advanced.md

#include <iostream>
#include <string>

#include <gtest/gtest.h>

// TEST() arguments go from general to specific. The first argument is the name of the test suite, and the second argument is the test's name within the test suite. Both names must be valid C++ identifiers, and they should not contain any underscores (_). A test's full name consists of its
// containing test suite and its individual name. Tests from different test suites can have the same individual name.

TEST(basic, test1) { // arg1 = test suite name, arg2 = test name
  EXPECT_EQ(1,1);
  EXPECT_NE(1,2);
  EXPECT_EQ(2,2);
}

TEST(basic, test2) {
  EXPECT_EQ(1.0,1.0);
}

TEST(basic, test3) {
  EXPECT_TRUE(true);
  EXPECT_FALSE(false);
  EXPECT_NE(1,2);
  EXPECT_LT(1,2);
  EXPECT_GT(2,1);
  using namespace std::string_literals;
  ASSERT_EQ(std::string("abc"), "abc"s);
  void *ptr = nullptr;
  ASSERT_EQ(ptr, nullptr);
  auto c_str1 = "rok";
  auto c_str2 = "rok";
  ASSERT_STREQ(c_str1, c_str2); // same content
//  ASSERT_NE(c_str1, c_str2); // different pointers (but can be optimized to point to the same location!)
}

int sqr(int x)
{
  return x*x;
}

TEST(sqr, positive) {
  ASSERT_EQ(sqr(2), 4);
}

template <typename E>
  class Queue {
  public:
    Queue() {}
    int f() { return 1; }
  };

class QueueTest : public ::testing::Test {
 protected:
 
  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

   Queue<int> q_;
};

TEST_F(QueueTest, f) {
  ASSERT_EQ(q_.f(), 1);
}

TEST(Floating, test1) {
  EXPECT_DOUBLE_EQ(1.0, 1.0);
  EXPECT_DOUBLE_EQ(1.0, 1.0+1e-17);
  EXPECT_NEAR(1.0, 1.0+1e-8, 1e-6);
}

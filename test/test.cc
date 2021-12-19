#include <gtest/gtest.h>

namespace {
int factorial(int x) {
    auto v = 1;
    for (; x; x -= 1)
        v *= x;
    return v;
}
} // namespace

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
    EXPECT_EQ(1, factorial(0));
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(1, factorial(1));
    EXPECT_EQ(2, factorial(2));
    EXPECT_EQ(6, factorial(3));
    EXPECT_EQ(40320, factorial(8));
}

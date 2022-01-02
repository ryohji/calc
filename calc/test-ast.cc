#include "ast.h"

#include <gtest/gtest.h>

#include <stdlib.h>

TEST(Syntax, number_zero) {
    auto node = node_new_number(0);
    EXPECT_EQ(0, node_value(node));
    node_release(node);
}

TEST(Syntax, number_100) {
    auto node = node_new_number(100);
    EXPECT_EQ(100, node_value(node));
    node_release(node);
}

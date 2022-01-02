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

TEST(Syntax, node_nagate_zero) {
    auto node0 = node_new_number(0);
    auto node = node_new_negation(node0);
    EXPECT_EQ(0, node_value(node));
    node_release(node0);
    node_release(node);
}

TEST(Syntax, node_nagate_1) {
    auto node1 = node_new_number(1);
    auto node = node_new_negation(node1);
    EXPECT_EQ(-1, node_value(node));
    node_release(node1);
    node_release(node);
}

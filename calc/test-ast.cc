#include "ast.h"

#include <functional>
#include <gtest/gtest.h>
#include <stdlib.h>

namespace {

char *write_stream(std::function<void(FILE *)> writer) {
    static char buffer[1024];
    FILE *fp = fmemopen(buffer, sizeof(buffer), "w");
    writer(fp);
    fclose(fp);
    return buffer;
}

} // namespace

TEST(Syntax, number_zero) {
    auto node = node_new_number(0);

    EXPECT_EQ(0, node_value(node));
    EXPECT_STREQ("0.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, number_100) {
    auto node = node_new_number(100);

    EXPECT_EQ(100, node_value(node));
    EXPECT_STREQ("100.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, node_nagate_zero) {
    auto node0 = node_new_number(0);
    auto node = node_new_negation(node0);
    node_release(node0);

    EXPECT_EQ(0, node_value(node));
    EXPECT_STREQ("negate 0.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, node_nagate_1) {
    auto node1 = node_new_number(1);
    auto node = node_new_negation(node1);
    node_release(node1);

    EXPECT_EQ(-1, node_value(node));
    EXPECT_STREQ("negate 1.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, node_plus_0_1) {
    auto node0 = node_new_number(0);
    auto node1 = node_new_number(1);
    auto node = node_new_addition(node0, node1);
    node_release(node0);
    node_release(node1);

    EXPECT_EQ(1, node_value(node));
    EXPECT_STREQ("+ 0.000000 1.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, node_minus_0_1) {
    auto node0 = node_new_number(0);
    auto node_ = node_new_number(1);
    auto node1 = node_new_negation(node_);
    node_release(node_);
    auto node = node_new_addition(node0, node1);
    node_release(node0);
    node_release(node1);

    EXPECT_EQ(-1, node_value(node));
    EXPECT_STREQ("+ 0.000000 negate 1.000000 ", write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

TEST(Syntax, node_plus_1_minus_0_1) {
    auto node0 = node_new_number(0);
    auto node1 = node_new_number(1);
    auto noden = node_new_negation(node1);
    auto node_ = node_new_addition(node0, noden);
    node_release(node0);
    node_release(noden);
    auto node = node_new_addition(node1, node_);
    node_release(node_);
    node_release(node1);

    EXPECT_EQ(0, node_value(node));
    EXPECT_STREQ("+ 1.000000 + 0.000000 negate 1.000000 ",
                 write_stream([&](FILE *fp) { node_fwrite(fp, node); }));

    node_release(node);
}

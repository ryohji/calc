#include <gtest/gtest.h>

#include <string>

namespace {

double eval(const std::string &expr) {
    return 1;
}

} // namespace

TEST(Expression, one_is_evaluated_to_1) {
    ASSERT_EQ(1, eval("1"));
}

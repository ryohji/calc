#include <gtest/gtest.h>

#include <string>

namespace {

double eval(const std::string &expr) {
    char *p;
    return std::strtod(expr.c_str(), &p);
}

} // namespace

TEST(Expression, zero_is_evaluated_to_0) {
    ASSERT_EQ(0, eval("0"));
}

TEST(Expression, one_is_evaluated_to_1) {
    ASSERT_EQ(1, eval("1"));
}

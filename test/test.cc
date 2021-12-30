#include <gtest/gtest.h>

#include "calc/calc.h"

#include <list>
#include <string>

TEST(Expression, zero_is_evaluated_to_0) {
    ASSERT_EQ(0, calc::eval("0"));
}

TEST(Expression, one_is_evaluated_to_1) {
    ASSERT_EQ(1, calc::eval("1"));
}

TEST(Expression, eval_plus_0_1) {
    ASSERT_EQ(1, calc::eval("+ 0 1"));
}

TEST(Expression, eval_minus_0_1) {
    ASSERT_EQ(-1, calc::eval("- 0 1"));
}

TEST(Expression, eval_plus_1_minus_0_1) {
    ASSERT_EQ(0, calc::eval("+ 1 - 0 1"));
}

TEST(Expression, tokenize_plus_0_1) {
    const auto list = std::list<std::string>{"+", "0", "1"};
    ASSERT_EQ(list, calc::detail::tokenize("+ 0 1"));
}

TEST(Expression, tokenize_plus_1_1) {
    const auto list = std::list<std::string>{"+", "1", "1"};
    ASSERT_EQ(list, calc::detail::tokenize("+ 1 1"));
}

TEST(Expression, tokenize_plus_minus_0_1_1) {
    const auto list = std::list<std::string>{"+", "-", "0", "1", "1"};
    ASSERT_EQ(list, calc::detail::tokenize("+  -  0  1  1  "));
}

TEST(Expression, evaluate_parsed_func_plus_1_minus_0_1) {
    const auto list = std::list<std::string>{"+", "1", "-", "0", "1"};
    ASSERT_EQ(0, calc::detail::parse(std::begin(list), std::end(list)).eval());
}

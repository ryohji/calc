#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <regex>
#include <sstream>
#include <string>

namespace {

std::list<std::string> tokenize(const std::string &input) {
    auto ws = std::regex("\\s+"); // to be allocated outside of the regex_token_iterator
    auto begin = std::sregex_token_iterator(std::begin(input), std::end(input), ws, -1);
    return std::list<std::string>(begin, std::sregex_token_iterator());
}

template <typename FwrdIt>
struct parsed_t {
    FwrdIt iter;
    std::function<double()> eval;
};

template <typename FwrdIt>
parsed_t<FwrdIt> parse(FwrdIt begin, FwrdIt end) {
    if (begin == end) {
        throw std::invalid_argument("Insufficient input.");
    } else {
        auto token = *begin;
        if (token.compare("+") == 0) {
            auto a = parse(++begin, end);
            auto b = parse(a.iter, end);
            return {b.iter, [=] { return a.eval() + b.eval(); }};
        } else if (token.compare("-") == 0) {
            auto a = parse(++begin, end);
            auto b = parse(a.iter, end);
            return {b.iter, [=] { return a.eval() - b.eval(); }};
        } else {
            char *p;
            const auto value = std::strtod(token.c_str(), &p);
            if (p == token.c_str() + token.length()) {
                return {++begin, [=] { return value; }};
            } else {
                auto os = std::ostringstream();
                os << "Not all token translated into a number: " << token;
                throw std::invalid_argument(os.str());
            }
        }
    }
}

double eval(const std::string &expr) {
    auto list = tokenize(expr);
    auto parsed = parse(std::begin(list), std::end(list));
    if (parsed.iter == std::end(list)) {
        return parsed.eval();
    } else {
        auto os = std::ostringstream();
        os << "Not all expression translated into a number: " << expr;
        throw std::invalid_argument(os.str());
    }
}

} // namespace

TEST(Expression, zero_is_evaluated_to_0) {
    ASSERT_EQ(0, eval("0"));
}

TEST(Expression, one_is_evaluated_to_1) {
    ASSERT_EQ(1, eval("1"));
}

TEST(Expression, eval_plus_0_1) {
    ASSERT_EQ(1, eval("+ 0 1"));
}

TEST(Expression, eval_minus_0_1) {
    ASSERT_EQ(-1, eval("- 0 1"));
}

TEST(Expression, eval_plus_1_minus_0_1) {
    ASSERT_EQ(0, eval("+ 1 - 0 1"));
}

TEST(Expression, tokenize_plus_0_1) {
    const auto list = std::list<std::string>{"+", "0", "1"};
    ASSERT_EQ(list, tokenize("+ 0 1"));
}

TEST(Expression, tokenize_plus_1_1) {
    const auto list = std::list<std::string>{"+", "1", "1"};
    ASSERT_EQ(list, tokenize("+ 1 1"));
}

TEST(Expression, tokenize_plus_minus_0_1_1) {
    const auto list = std::list<std::string>{"+", "-", "0", "1", "1"};
    ASSERT_EQ(list, tokenize("+  -  0  1  1  "));
}

TEST(Expression, evaluate_parsed_func_plus_1_minus_0_1) {
    const auto list = std::list<std::string>{"+", "1", "-", "0", "1"};
    ASSERT_EQ(0, parse(std::begin(list), std::end(list)).eval());
}

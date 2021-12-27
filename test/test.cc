#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <list>
#include <regex>
#include <sstream>
#include <string>

namespace {

class expr;

typedef std::shared_ptr<const expr> expr_ptr;

std::list<std::string> tokenize(const std::string &input) {
    auto ws = std::regex("\\s+"); // to be allocated outside of the regex_token_iterator
    auto begin = std::sregex_token_iterator(std::begin(input), std::end(input), ws, -1);
    return std::list<std::string>(begin, std::sregex_token_iterator());
}

class expr {
  public:
    virtual ~expr() {}
    virtual double value() const = 0;
};

class number : public expr {
    const double v;

  public:
    number(double v) : v(v) {}
    double value() const { return v; }
};

class nagate : public expr {
    const expr_ptr v;

  public:
    nagate(const expr_ptr v) : v(v) {}
    double value() const { return -v->value(); }
};

class plus : public expr {
    const expr_ptr a, b;

  public:
    plus(const expr_ptr a, const expr_ptr b) : a(a), b(b) {}
    double value() const { return a->value() + b->value(); }
};

class minus : public plus {
  public:
    minus(const expr_ptr a, const expr_ptr b) : plus(a, expr_ptr(new nagate(b))) {}
};

template <typename FwrdIt>
struct parsed_type {
    FwrdIt iter;
    expr_ptr expr;
};

template <typename FwrdIt>
parsed_type<FwrdIt> parse(FwrdIt begin, FwrdIt end) {
    if (begin == end) {
        throw std::invalid_argument("Insufficient input.");
    } else {
        auto token = *begin;
        if (token.compare("+") == 0) {
            auto a = parse(++begin, end);
            auto b = parse(a.iter, end);
            return {b.iter, expr_ptr(new plus(a.expr, b.expr))};
        } else if (token.compare("-") == 0) {
            auto a = parse(++begin, end);
            auto b = parse(a.iter, end);
            return {b.iter, expr_ptr(new minus(a.expr, b.expr))};
        } else {
            char *p;
            const auto value = std::strtod(token.c_str(), &p);
            if (p == token.c_str() + token.length()) {
                return {++begin, expr_ptr(new number(value))};
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
    auto end = std::end(list);
    auto parsed = parse(std::begin(list), end);
    if (parsed.iter == end) {
        return parsed.expr->value();
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

TEST(Expression, eval_plus_minus_0_1_1) {
    ASSERT_EQ(0, eval("+ - 0 1 1"));
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
    ASSERT_EQ(list, tokenize("+  - 0 1  1"));
}

TEST(Expression, evaluate_number_zero) {
    ASSERT_EQ(0, number(0).value());
}

TEST(Expression, evaluete_number_100) {
    ASSERT_EQ(100, number(100).value());
}

TEST(Expression, evaluete_plus_0_0) {
    auto a = expr_ptr(new number(0));
    auto b = expr_ptr(new number(0));
    ASSERT_EQ(0, plus(a, b).value());
}

TEST(Expression, evaluete_plus_comma1_comma1) {
    auto a = expr_ptr(new number(0.1));
    auto b = expr_ptr(new number(0.1));
    ASSERT_EQ(0.2, plus(a, b).value());
}

TEST(Expression, evaluete_plus_minus_0_1_1) {
    auto o = expr_ptr(new number(0));
    auto e = expr_ptr(new number(1));
    auto m = expr_ptr(new minus(o, e));
    ASSERT_EQ(0, plus(m, e).value());
}

TEST(Expression, evaluate_parsed_plus_minus_0_1_1) {
    const auto list = std::list<std::string>{"+", "-", "0", "1", "1"};
    ASSERT_EQ(0, parse(std::begin(list), std::end(list)).expr->value());
}

#include "calc.h"

#include <regex>

double calc::eval(const std::string &expr) {
    auto list = detail::tokenize(expr);
    auto parsed = detail::parse(std::begin(list), std::end(list));
    if (parsed.iter == std::end(list)) {
        return parsed.eval();
    } else {
        auto os = std::ostringstream();
        os << "Not all expression translated into a number: " << expr;
        throw std::invalid_argument(os.str());
    }
}

namespace calc {
namespace detail {

std::list<std::string> tokenize(const std::string &input) {
    auto ws = std::regex("\\s+"); // to be allocated outside of the regex_token_iterator
    auto begin = std::sregex_token_iterator(std::begin(input), std::end(input), ws, -1);
    return std::list<std::string>(begin, std::sregex_token_iterator());
}

double parse_as_double(const std::string &expr) {
    char *p;
    const auto value = std::strtod(expr.c_str(), &p);
    if (p == expr.c_str() + expr.length()) {
        return value;
    } else {
        auto os = std::ostringstream();
        os << "Not all token translated into a number: " << expr
           << "; remainder: " << p;
        throw std::invalid_argument(os.str());
    }
}

std::map<std::string, std::function<double(evaluatable, evaluatable)>> bin_ops = {
    {"+", [](evaluatable a, evaluatable b) { return a() + b(); }},
    {"-", [](evaluatable a, evaluatable b) { return a() - b(); }},
};

} // namespace detail
} // namespace calc

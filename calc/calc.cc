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

std::list<std::string> calc::detail::tokenize(const std::string &input) {
    auto ws = std::regex("\\s+"); // to be allocated outside of the regex_token_iterator
    auto begin = std::sregex_token_iterator(std::begin(input), std::end(input), ws, -1);
    return std::list<std::string>(begin, std::sregex_token_iterator());
}

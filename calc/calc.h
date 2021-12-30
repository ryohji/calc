#pragma once

#include <exception>
#include <functional>
#include <list>
#include <sstream>
#include <string>

namespace calc {

double eval(const std::string &expr);

namespace detail {

std::list<std::string> tokenize(const std::string &input);

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

} // namespace detail

} // namespace calc

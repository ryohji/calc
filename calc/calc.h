#pragma once

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <sstream>
#include <string>

namespace calc {

double eval(const std::string &expr);

namespace detail {

typedef std::function<double()> evaluatable;

extern std::map<std::string, std::function<double(evaluatable, evaluatable)>> bin_ops;

std::list<std::string> tokenize(const std::string &input);

double parse_as_double(const std::string &expr);

template <typename FwrdIt>
struct parsed_t {
    FwrdIt iter;
    evaluatable eval;
};

template <typename FwrdIt>
parsed_t<FwrdIt> parse(FwrdIt begin, FwrdIt end) {
    if (begin == end) {
        throw std::invalid_argument("Insufficient input.");
    } else {
        const auto found = bin_ops.find(*begin);
        if (found != std::end(bin_ops)) {
            const auto f = found->second;
            const auto a = parse(++begin, end);
            const auto b = parse(a.iter, end);
            return {b.iter, [=] { return f(a.eval, b.eval); }};
        } else {
            const auto value = parse_as_double(*begin);
            return {++begin, [=] { return value; }};
        }
    }
}

} // namespace detail

} // namespace calc

#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <random>
#include <cmath>
#include <cstddef>

namespace detail {

template <typename T>
bool values_equal(const T& a, const T& b) {
    return a == b;
}

template <>
inline bool values_equal<float>(const float& a, const float& b) {
    return std::fabs(a - b) < 1e-9f;
}

template <>
inline bool values_equal<double>(const double& a, const double& b) {
    return std::fabs(a - b) < 1e-9;
}

template <typename T>
T generate_random(const T& lower, const T& upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(
        static_cast<double>(lower), static_cast<double>(upper));
    return static_cast<T>(dist(gen));
}

template <>
inline int generate_random<int>(const int& lower, const int& upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(lower, upper);
    return dist(gen);
}

template <>
inline char generate_random<char>(const char& lower, const char& upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(
        static_cast<int>(lower), static_cast<int>(upper));
    return static_cast<char>(dist(gen));
}

template <>
inline std::string generate_random<std::string>(
    const std::string& lower, const std::string& upper) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::size_t min_len = lower.empty() ? 1 : lower.size();
    std::size_t max_len = upper.empty() ? 10 : upper.size();
    if (min_len > max_len) std::swap(min_len, max_len);
    std::uniform_int_distribution<std::size_t> len_dist(min_len, max_len);
    std::size_t len = len_dist(gen);
    std::string result;
    result.reserve(len);
    for (std::size_t i = 0; i < len; ++i) {
        std::uniform_int_distribution<int> char_dist('a', 'z');
        result += static_cast<char>(char_dist(gen));
    }
    return result;
}

} // namespace detail


#endif

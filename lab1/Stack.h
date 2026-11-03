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

template <typename T>
class Stack {
public:
    static constexpr double kEpsilon = 1e-9;

    Stack()
        : data_(nullptr), size_(0), capacity_(0) {}

    Stack(std::size_t count, const T& lower, const T& upper)
        : data_(nullptr), size_(0), capacity_(0) {
        for (std::size_t i = 0; i < count; ++i) {
            *this += detail::generate_random(lower, upper);
        }
    }

    Stack(const Stack& other)
        : data_(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            size_ = other.size_;
            capacity_ = other.capacity_;
            if (capacity_ > 0) {
                data_ = new T[capacity_];
                for (std::size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }
            }
        }
        return *this;
    }

    ~Stack() {
        clear();
    }

    Stack& operator+=(const T& value) {
        if (size_ == capacity_) {
            resize(capacity_ == 0 ? 4 : capacity_ * 2);
        }
        data_[size_++] = value;
        return *this;
    }

    Stack& operator-=(int) {
        if (size_ == 0) {
            throw std::runtime_error("Stack::pop: stack is empty");
        }
        --size_;
        return *this;
    }

    T top() const {
        if (size_ == 0) {
            throw std::runtime_error("Stack::top: stack is empty");
        }
        return data_[size_ - 1];
    }

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    bool operator==(const Stack& other) const {
        if (size_ != other.size_) return false;
        for (std::size_t i = 0; i < size_; ++i) {
            if (!detail::values_equal(data_[i], other.data_[i])) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Stack& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Stack& stack) {
        os << "Stack[" << stack.size_ << "]: {";
        for (std::size_t i = 0; i < stack.size_; ++i) {
            os << stack.data_[i];
            if (i + 1 < stack.size_) os << ", ";
        }
        os << "}";
        return os;
    }

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    void resize(std::size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void clear() {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }
};

#endif
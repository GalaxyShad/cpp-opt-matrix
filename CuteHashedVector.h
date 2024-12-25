#ifndef CPP_OPT_MATRIX_CUTEHASHEDVECTOR_H
#define CPP_OPT_MATRIX_CUTEHASHEDVECTOR_H

#include <unordered_map>
#include <format>

#include "ICuteVector.h"

template<typename T>
class CuteVectorHashed : public ICuteVector<T> {
private:
    std::unordered_map<int, T> map_;
    size_t size_;

public:
    explicit CuteVectorHashed(size_t size) : size_(size) {}

public:
    void set(int i, T val) override {
        checkOutOfRange(i);

        if (val == T(0)) {
            map_.erase(i);
            return;
        }

        map_[i] = val;
    }

    T get(int i) override {
        checkOutOfRange(i);

        return map_.contains(i) ? map_[i] : T(0);
    }

    size_t size() override { return size_; }

    T dot_product(ICuteVector<T> &other) override {
        if (size_ != other.size()) {
            throw std::invalid_argument("Vectors must be of the same size");
        }

        T result = T(0);
        for (size_t i = 0; i < size_; ++i) {
            result += get(i) * other.get(i);
        }
        return result;
    }

    void add(ICuteVector<T> &other) override {
        if (size_ != other.size()) {
            throw std::invalid_argument("Vectors must be of the same size");
        }

        for (size_t i = 0; i < size_; ++i) {
            set(i, get(i) + other.get(i));
        }
    }

    void add_scalar(T scalar) override {
        for (size_t i = 0; i < size_; ++i) {
            set(i, get(i) + scalar);
        }
    }

    void subtract_scalar(T scalar) override {
        for (size_t i = 0; i < size_; ++i) {
            set(i, get(i) - scalar);
        }
    }

    void mult_scalar(T scalar) override {
        for (size_t i = 0; i < size_; ++i) {
            set(i, get(i) * scalar);
        }
    }

    void div_scalar(T scalar) override {
        if (scalar == T(0)) {
            throw std::invalid_argument("Division by zero");
        }

        for (size_t i = 0; i < size_; ++i) {
            set(i, get(i) / scalar);
        }
    }

    void pow_scalar(T scalar) override {
        for (size_t i = 0; i < size_; ++i) {
            set(i, std::pow(get(i), scalar));
        }
    }


private:
    void checkOutOfRange(int i) {
        if (i >= 0 && i < size_) return;

        throw std::out_of_range(std::format("Index {} is out of range, min index = 0, max index = {}", i, size_));
    }
};

#endif//CPP_OPT_MATRIX_CUTEHASHEDVECTOR_H

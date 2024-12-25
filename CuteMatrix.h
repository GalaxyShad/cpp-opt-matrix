#ifndef CPP_OPT_MATRIX_CUTEMATRIX_H
#define CPP_OPT_MATRIX_CUTEMATRIX_H

#include <unordered_map>

#include "CuteHashedVector.h"
#include "CuteStdVector.h"

template<typename T>
class CuteMatrix {
private:
    std::unique_ptr<ICuteVector<T>> vector_ptr_;
    ICuteVector<T> &vector_;
    bool isUsingStdVector_;
    size_t width_, height_;

public:
    explicit CuteMatrix(size_t width, size_t height, bool useStdVector = false)
        : isUsingStdVector_(useStdVector),
          vector_ptr_(useStdVector
                              ? (ICuteVector<T>*)(new CuteVectorHashed<T>(width * height))
                              : (ICuteVector<T>*)(new CuteStdVector<T>(width * height))),
          vector_(*vector_ptr_),
          width_(width),
          height_(height) {}

public:
    void set(int x, int y, T val) {
        vector_.set(y * width_ + x, val);
    }

    T get(int x, int y) {
        return vector_.get(y * width_ + x);
    }

    size_t width() const {
        return width_;
    }

    size_t height() const {
        return height_;
    }

    CuteMatrix<T> transpose() {
        CuteMatrix<T> result(height_, width_);

        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(j, i, get(i, j));
            }
        }

        return result;
    }

    CuteMatrix<T> operator+(const CuteMatrix<T> &other) const {
        if (width_ != other.width_ || height_ != other.height_) {
            throw std::invalid_argument("Matrices must be of the same size");
        }

        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, get(i, j) + other.get(i, j));
            }
        }
        return result;
    }

    CuteMatrix<T> operator*(const CuteMatrix<T> &other) const {
        if (width_ != other.height_) {
            throw std::invalid_argument("Incompatible matrix dimensions for multiplication");
        }

        CuteMatrix<T> result(other.width_, height_);
        for (size_t i = 0; i < height_; ++i) {
            for (size_t j = 0; j < other.width_; ++j) {
                T sum = T(0);
                for (size_t k = 0; k < width_; ++k) {
                    sum += get(k, i) * other.get(j, k);
                }
                result.set(j, i, sum);
            }
        }
        return result;
    }

    CuteMatrix<T> operator+(T scalar) const {
        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, get(i, j) + scalar);
            }
        }
        return result;
    }

    CuteMatrix<T> operator-(T scalar) const {
        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, get(i, j) - scalar);
            }
        }
        return result;
    }

    CuteMatrix<T> operator*(T scalar) const {
        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, get(i, j) * scalar);
            }
        }
        return result;
    }

    CuteMatrix<T> operator/(T scalar) const {
        if (scalar == T(0)) {
            throw std::invalid_argument("Division by zero");
        }
        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, get(i, j) / scalar);
            }
        }
        return result;
    }

    CuteMatrix<T> operator^(T exponent) const {
        CuteMatrix<T> result(width_, height_);
        for (size_t i = 0; i < width_; ++i) {
            for (size_t j = 0; j < height_; ++j) {
                result.set(i, j, std::pow(get(i, j), exponent));
            }
        }
        return result;
    }

    CuteMatrix<T> pow_elements(int exponent) const {
        if (width_ != height_) {
            throw std::invalid_argument("Matrix must be square to be raised to a power");
        }

        if (exponent == 0) {
            CuteMatrix<T> result(width_, height_);
            for (size_t i = 0; i < width_; ++i) {
                result.set(i, i, T(1));
            }
            return result;
        }

        if (exponent < 0) {
            return inverse().pow(-exponent);
        }

        CuteMatrix<T> result = *this;
        CuteMatrix<T> base = *this;

        exponent--;
        while (exponent > 0) {
            if (exponent % 2 == 1) {
                result = result * base;
            }
            base = base * base;
            exponent /= 2;
        }

        return result;
    }

    CuteMatrix<T> inverse() {
        if (width_ != height_) {
            throw std::invalid_argument("Matrix must be square to be invertible");
        }

        size_t n = width_;
        CuteMatrix<T> result(n, n, isUsingStdVector_);
        CuteMatrix<T> temp(n, n, isUsingStdVector_);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                temp.set(i, j, get(i, j));
                result.set(i, j, (i == j) ? T(1) : T(0));
            }
        }

        for (size_t i = 0; i < n; ++i) {
            // Find pivot
            T pivot = temp.get(i, i);
            if (pivot == T(0)) {
                throw std::runtime_error("Matrix is singular and cannot be inverted");
            }

            for (size_t j = 0; j < n; ++j) {
                if (i != j) {
                    T factor = temp.get(j, i) / pivot;

                    for (size_t k = 0; k < n; ++k) {
                        temp.set(j, k, temp.get(j, k) - temp.get(i, k) * factor);
                        result.set(j, k, result.get(j, k) - result.get(i, k) * factor);
                    }
                }
            }
        }

        for (size_t i = 0; i < n; ++i) {
            T pivot = temp.get(i, i);
            for (size_t j = 0; j < n; ++j) {
                result.set(i, j, result.get(i, j) / pivot);
            }
        }

        return result;
    }
};

#endif//CPP_OPT_MATRIX_CUTEMATRIX_H

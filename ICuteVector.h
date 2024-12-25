#ifndef CPP_OPT_MATRIX_ICUTEVECTOR_H
#define CPP_OPT_MATRIX_ICUTEVECTOR_H

#include <cstdlib>

template<typename T>
struct ICuteVector {
    virtual void set(int i, T val) = 0;
    virtual T get(int i) = 0;
    virtual size_t size() = 0;

    virtual T dot_product(ICuteVector<T> &other) = 0;
    virtual void add(ICuteVector<T> &other) = 0;

    virtual void add_scalar(T scalar) = 0;
    virtual void subtract_scalar(T scalar) = 0;
    virtual void mult_scalar(T scalar) = 0;
    virtual void div_scalar(T scalar) = 0;
    virtual void pow_scalar(T scalar) = 0;

    virtual ~ICuteVector() = default;
};

#endif//CPP_OPT_MATRIX_ICUTEVECTOR_H

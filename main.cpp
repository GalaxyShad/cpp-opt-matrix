#include <iostream>
#include <print>

#include <functional>
#include <chrono>

#include "CuteMatrix.h"

template<typename T>
void print_matrix(CuteMatrix<T>& m) {
    for (size_t i = 0; i < m.height(); ++i) {
        for (size_t j = 0; j < m.width(); ++j) {
            std::print("{} ", m.get(j, i));
        }
        std::println("");
    }
}

template<typename T>
void print_vector(ICuteVector<T>& v) {
    for (int i = 0; i < v.size(); ++i) {
        std::print("{} ", v.get(i));
    }
    std::println("");
}

void test_transpose() {
    CuteMatrix<int> m(2, 2);
    m.set(1, 1, 25);
    m.set(0, 0, 1);
    m.set(0, 1, 2);
    m.set(1, 0, 3);
    std::println("Original matrix:");
    print_matrix(m);

    CuteMatrix<int> transposed = m.transpose();
    std::println("Transposed matrix:");
    print_matrix(transposed);
}

void test_inverse() {
    CuteMatrix<double> m(2, 2);

    m.set(0, 0, 1);
    m.set(0, 1, 2);
    m.set(1, 0, 3);
    m.set(1, 1, 4);

    std::println("Original matrix:");
    print_matrix(m);

    CuteMatrix<double> inverse = m.inverse();

    std::println("Inverse matrix:");
    print_matrix(inverse);
}

void test_vector_add() {
    CuteVectorHashed<int> v1(2);
    v1.set(0, 1);
    v1.set(1, 2);
    std::println("Vector 1:");
    print_vector(v1);

    CuteVectorHashed<int> v2(2);
    v2.set(0, 3);
    v2.set(1, 4);
    std::println("Vector 2:");
    print_vector(v2);

    v1.add(v2);
    std::println("Vector addition:");
    print_vector(v1);
}

void test_vector_dot_product() {
    CuteVectorHashed<int> v1(2);
    v1.set(0, 1);
    v1.set(1, 2);
    std::println("Vector 1:");
    print_vector(v1);

    CuteVectorHashed<int> v2(2);
    v2.set(0, 3);
    v2.set(1, 4);
    std::println("Vector 2:");
    print_vector(v2);

    auto dotProduct = v1.dot_product(v2);
    std::println("Dot product: {}", dotProduct);
}

void test_inverse_speed() {
    constexpr int size = 300;

    CuteMatrix<int> m(size, size);
    CuteMatrix<int> m_std(size, size, true);

    for (int i = 0; i < m.height(); i++) {
        for (int j = 0; j < m.width(); j++) {
            auto val = rand();

            m.set(j, i, val);
            m_std.set(j, i, val);
        }
    }

    std::println("Matrix Size {}x{}", m.width(), m.height());

    auto start = std::chrono::high_resolution_clock::now();
    m.inverse();
    auto duration = std::chrono::high_resolution_clock::now() - start;
    std::println("HashMap Matrix        {}", std::chrono::duration_cast<std::chrono::milliseconds>(duration));

    auto start_std = std::chrono::high_resolution_clock::now();
    m_std.inverse();
    auto duration_std = std::chrono::high_resolution_clock::now() - start_std;
    std::println("Std Vector Matrix     {}", std::chrono::duration_cast<std::chrono::milliseconds>(duration_std));
}

int main() {
    std::vector<std::pair<std::string, std::function<void()>>> tests = {
            {"Matrix Transpose", test_transpose},
            {"Matrix Inverse", test_inverse},
            {"Vector add", test_vector_add},
            {"Vector dot product", test_vector_dot_product},
            {"Test with std matrix", test_inverse_speed},
    };

    for (auto& t : tests) {
        std::println("=== {} ===", t.first);
        t.second();
    }

    return 0;
}

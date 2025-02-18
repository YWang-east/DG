#include <DG.hpp>
#include "core/basis.hpp"

using namespace DG;

bool test_vector() {
    /*
        assignment tests
    */
    vec_t a = {1.0, 2.0, 3.0, 4.0, 5.0};
    vec_t b = a;
    vec<size_t> c(5);

    c = 1;

    b[2] = 100;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    /*
        operations tests
    */
    vec_t a1 = a * 2.0;
    vec_t a2 = a + a1;
    vec_t a3 = a - a1;
    std::cout << a << std::endl;
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
    std::cout << a3 << std::endl;

    return 0;
}

bool test_matrix() {
    /*
        assignment tests
    */
    mat_t A(2, 3);
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 3; j++) {
            A(i, j) = i + j;
        }
    }

    /*
        transpose tests
    */
    mat_t B = {{1.0, 2.0, 3.0, 4.0}, {4.0, 5.0, 6.0, 7.0}, {7.0, 8.0, 9.0, 10.0}};

    std::cout << B << std::endl;
    std::cout << B.transpose() << std::endl;

    /*
        inversion test
    */
    mat_t C = {{4.0, 7.0, 2.0}, {9.0, 2.0, 1.0}, {3.0, 5.0, 6.0}};

    std::cout << C.inv() << std::endl;
    std::cout << C * C.inv() << std::endl;

    /*
        operations tests
    */
    mat_t A1 = A * 2.0;
    mat_t A2 = A / 2.0;

    std::cout << A << std::endl;
    std::cout << A1 << std::endl;
    std::cout << A2 << std::endl;
    return 0;
}

bool test_basis() {
    RefCell c(5);
    std::cout << c.x << std::endl;
    std::cout << c.w << std::endl;

    std::cout << c.M << std::endl;
    std::cout << c.D << std::endl;
    
    return 0;
}

int main() {    
    // if (test_vector()) {
    //     std::cout << "Vector test failed!" << std::endl;
    //     return 1;
    // }

    // if (test_matrix()) {
    //     std::cout << "Matrix test failed!" << std::endl;
    //     return 1;
    // }

    if (test_basis()) {
        std::cout << "Basis test failed!" << std::endl;
        return 1;
    }

    std::cout << "All tests passed!" << std::endl;

    return 0;
}


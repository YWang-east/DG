#pragma once

#include <cstddef>
#include <numbers>
#include "types.hpp"

namespace DG::core{
    /**
     *  @brief the Legendre polynomials
     */
    struct Legendre {
        static scalar_t poly (scalar_t x, size_t n) {
            if (n == 0) return 1.0;
            if (n == 1) return x;
            return (2.0*n-1.0)/n * x * poly(x, n-1) - (n-1.0)/n * poly(x, n-2);
        }

        static scalar_t d_poly (scalar_t x, size_t n) {
            if (n == 0) return 0.0;
            return n * poly(x, n-1) + x * d_poly(x, n - 1);
        }

        static scalar_t d2_poly (scalar_t x, size_t n) {
            if (n == 0) return 0.0;
            if (n == 1) return 0.0;
            return (n + 1) * d_poly(x, n-1) + x * d2_poly(x, n - 1);
        }
    };

    /**
     *  @brief the base quadrature class
     */
    class Quadrature {
    public:
        size_t n;   // number of quadrature points
        vec_t x;
        vec_t w;

        Quadrature(size_t n) : n(n), x(n), w(n) {}

        virtual void print() {
            std::cout << "Quadrature" << std::endl;
            std::cout << "----------" << std::endl;
            std::cout << "coordinates:\t" << x << std::endl;
            std::cout << "weights:\t" << w << std::endl;
        }

        virtual ~Quadrature() = default;
    };

    /**
     *  @brief Chebyshev quadrature of the first kind (include end points)
     */
    class Chebyshev1 : public Quadrature{
    public:
        Chebyshev1(size_t n) : Quadrature(n) {
            for (size_t i = 0; i < n; i++) {
                x[i] = -std::cos(std::numbers::pi * i / n);
            }
        }

        ~Chebyshev1() = default;
    };

    /**
     *  @brief Chebyshev quadrature of the second kind (exclude end points)
     */
    class Chebyshev2 : public Quadrature{
    public:
        Chebyshev2(size_t n) : Quadrature(n) {
            for (size_t i = 0; i < n; i++) {
                x[i] = -std::cos(std::numbers::pi * (4 * i + 3) / (4 * n + 2));
            }
        }

        ~Chebyshev2() = default;
    };

    /**
     *  @brief Gauss-Legendre quadrature (exclude end points)
     */
    class Gaussian : public Quadrature{
    public:
        // compute Gaussian quadrature points and weights using Newton-Rapson
        Gaussian(size_t n) : Quadrature(n) {
            // initial guess
            Chebyshev2 Cheb2Q(n);
            x = Cheb2Q.x;

            vec_t x_prev(n);
            x_prev = 2.0;
            vec_t x_diff = x - x_prev;

            int count = 0;
            int max_iter = 100;

            while ( x_diff.Linf_norm() > 1e-10 || count < max_iter) {
                x_prev = x;
                for (size_t i = 0; i < n; i++) {
                    x[i] = x_prev[i] - Legendre::poly(x_prev[i], n) / Legendre::d_poly(x_prev[i], n);
                }
                x_diff = x - x_prev;
                count++;
            }

            // compute weights
            for (size_t i = 0; i < n; i++) {
                w[i] = 2.0 / ((1.0 - x[i] * x[i]) * Legendre::d_poly(x[i], n) * Legendre::d_poly(x[i], n));
            }
        }

        ~Gaussian() = default;
    };

    /**
     *  @brief Gauss-Lobatto quadrature (include end points)
     */
    class Lobatto : public Quadrature{
    public:
        Lobatto(size_t n) : Quadrature(n) {
            // initial guess
            Chebyshev2 Cheb2Q(n-2);
            for (size_t i = 1; i < n - 1; i++) {
                x[i] = Cheb2Q.x[i-1];
            }
            x[0] = -1;
            x[n-1] = 1;

            vec_t x_prev(n);
            x_prev = 2.0;
            vec_t x_diff = x - x_prev;

            int count = 0;
            int max_iter = 100;

            while ( x_diff.Linf_norm() > 1e-10 || count < max_iter) {
                x_prev = x;
                for (size_t i = 1; i < n - 1; i++) {
                    x[i] = x_prev[i] - Legendre::d_poly(x_prev[i], n-1) / Legendre::d2_poly(x_prev[i], n-1);
                }
                x_diff = x - x_prev;
                count++;
            }

            // compute weights
            for (size_t i = 0; i < n; i++) {
                if (i==0 || i==n-1)
                    w[i] = 2.0 / (n * (n - 1));
                else
                    w[i] = 2.0 / (n * (n - 1) * Legendre::poly(x[i], n-1) * Legendre::poly(x[i], n-1));
            }
        }

        ~Lobatto() = default;
    };
}
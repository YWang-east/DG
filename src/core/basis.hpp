#pragma once

#include "core/quadrature.hpp"
#include "types.hpp"
#include <cstddef>

namespace DG::core {
    /**
     *  @brief the reference cell 
     */
    struct RefCell {
        size_t n;       // number of quadrature points
        vec_t x;        // Coordinates of quadrature points
        vec_t w;        // Weights of quadrature points

        mat_t V;        // Vandermonde matrix
        mat_t dV;       // Gradient of Vandermonde matrix
        mat_t M;        // Mass matrix
        mat_t Minv;     // Inverse of mass matrix
        mat_t D;        // Differentiation matrix
        mat_t B;        // Boundary matrix
        mat_t MinvB;    // Inverse of mass matrix times the boundary matrix

        // constructor: compute all matrices
        RefCell(size_t n) : n(n), x(n), w(n), V(n, n), dV(n, n), M(n, n), Minv(n, n), D(n, n), B(n, n), MinvB(n, n)
        {
            // quadrature. TODO: could template over quadrature type
            Lobatto Q(n);
            x = Q.x; w = Q.w;

            // Vandermonde matrix
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < n; j++) {
                    V(i, j)  = std::sqrt(0.5*(2.0*j + 1.0)) * Legendre::poly(x[i], j);
                    dV(i, j) = std::sqrt(0.5*(2.0*j + 1.0)) * Legendre::d_poly(x[i], j);
                }
            }

            // Mass matrix
            Minv = V * V.transpose();
            M = Minv.inv();

            // Differentiation matrix
            D = dV * V.inv();

            // Boundary matrix
            B = 0.0;
            B(0, 0) = -1.0; 
            B(n-1, n-1) = 1.0;

            MinvB = Minv * B;
        }

        ~RefCell() = default;
    };

}
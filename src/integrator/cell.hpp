#pragma once

#include "core/types.hpp"
#include <DG.hpp>

namespace DG::integrator{

    enum BoundaryFlux {
        L,
        R
    };

    /**
     *  @brief DG cell class
     */
    template<typename Model>
    class Cell {
    private:
        static constexpr size_t ND = Model::NumDims;
        static constexpr size_t NE = Model::NumEqns;
        
    public:
        scalar_t detJ;
        arr_t<ND> size;
        vec<arr_t<ND>> x;   // coordinates of the quadrature points

        mat_t u;
        mat_t p;
        mat_t u0;

        std::array<vec_t, 2> f_star;    // numerical fluxes at cell boundaries

        /*
            constructor: initialize a cell
        */
        Cell(size_t Nnodes) : x(Nnodes), u(Nnodes, NE), p(Nnodes, NE), u0(Nnodes, NE) {
            f_star[L].resize(NE);
            f_star[R].resize(NE);
        }
    };
}
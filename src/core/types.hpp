#pragma once

#include "matrix.hpp"
#include "array.hpp"

namespace DG::core{
    /* scalar type; easy switch between single and double precision */
    using scalar_t = double;

    /* type aliases for convenience */
    using vec_t = vec<scalar_t>;
    using mat_t = matrix<scalar_t>;
    template<const size_t N>
    using arr_t = arr<scalar_t, N>;

    enum FaceLocation {
        LEFT,
        RIGHT,
        INTER
    };
}
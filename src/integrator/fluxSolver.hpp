#pragma once

#include <DG.hpp>
#include <cstddef>

namespace DG::integrator::flux {

    template<typename Model>
    class LaxFriedrichs {

        static constexpr scalar_t a = 0.0;

    public:
        static vec_t flux(const vec_t &u_minus, const vec_t &u_plus, const size_t dim) {
            scalar_t lambda_l = Model::max_wave_speed(u_minus, dim);
            scalar_t lambda_r = Model::max_wave_speed(u_plus, dim);
            scalar_t alpha = std::max(lambda_l, lambda_r);

            vec_t flux = (Model::Fu(u_minus, dim) + Model::Fu(u_plus, dim)) * 0.5 - (u_plus - u_minus) * 0.5 * alpha * (1.0 - a);
            return flux;
        }
    };

    template<typename Model>
    class HLLC {

    public:

    };

    template<typename Model>
    class Linear {
        
    };

    template<typename Model>
    class Roe {
        
    };
}
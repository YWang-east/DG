#pragma once

#include <DG.hpp>
#include <cstddef>
#include "core/types.hpp"
#include "model.hpp"

namespace DG::model {

    /**
     * @brief Euler equations
     */
    template<const size_t ND>
    class Euler : public Model<ND>{
    public:
        /*
            model constants
        */
        static constexpr size_t NumEqns = ND + 2;   // mass, momentums and energy
        static constexpr size_t ip = ND + 1;   // pressure index
        static constexpr scalar_t gamma = 1.4;

        // for convenience
        // using var_t = arr_t<NumEqns>;

        /*
            compute flux in one dimension using conservative variables
        */
        static vec_t Fu(const vec_t &u, const size_t dim) {
            vec_t F(u.size());
            size_t in = 1 + dim;	        // normal velocity index

			for (size_t i = 0; i < ND; ++i) {
				F[1 + i] = u[in] * u[1 + i] / u[0];
			}

			F[0]   = u[in];
			F[in] += pressure(u);
			F[ip]  = u[in] / u[0] * (u[ip] + pressure(u));

			return F;
        }

        /*
            compute flux in one dimension using primitive variables
        */
        static vec_t Fp(const vec_t &p, const size_t dim) {
            vec_t F(p.size());
            size_t in = 1 + dim;	// normal velocity index
			scalar_t ke = 0.0;		// specific kinetic energy

			for (size_t i = 0; i < ND; ++i) {
				F[1 + i] = p[0] * p[in] * p[1 + i];
				ke += 0.5 * p[1 + i] * p[1 + i];
			}

			F[0]   = p[in] * p[0];
			F[in] += p[ip];
			F[ip]  = p[in] * (gamma/(gamma-1) * p[ip] + ke * p[0]);

			return F;
        }

        /*
            conserved to primitive variables
        */
        static vec_t UtoP (const vec_t &u) {
            vec_t p(u.size());

            p[0] = u[0];
			for (size_t i = 0; i < ND; ++i) {
				p[1 + i] = u[1 + i] / u[0];
			}
			p[ip] = pressure(u);

			return p;
        }

        /*
            primitive to conserved variables
        */
        static vec_t PtoU (const vec_t &p) {
            vec_t u(p.size());
            scalar_t ke = 0;

			u[0] = p[0];
			for (size_t i = 0; i < ND; ++i) {
				u[1 + i] = p[0] * p[1 + i];
				ke += 0.5 * p[1 + i] * p[1 + i];
			}
			u[ip] = p[ip] / (gamma - 1) + p[0] * ke;
            
            return u;
        }

        static scalar_t compute_dt(const auto &cell) {
            arr_t<ND> umax, dt;
            for (size_t i = 0; i < ND; i++) {
                umax[i] = 0.0;

                // find the max velocity in the cell
                for (size_t j = 0; j < cell.p.rows(); j++) {
                    umax[i] = std::max(umax[i], std::abs(cell.p(j, 1 + i)) + sound_speed(cell.p(j, 0), cell.p(j, ip)));
                }
                dt[i] = cell.size[i] / umax[i];
            }
            return dt.min();
        }

        /*
            pressure
        */
        static scalar_t pressure(const vec_t &u) {
			scalar_t ke = 0;
			for (size_t i = 0; i < ND; ++i) {
				ke += 0.5 * u[1 + i] * u[1 + i];
			}
			return (gamma - 1) * (u[ip] - ke / u[0]);
		}

        /*
            speed of sound
        */
        inline static scalar_t sound_speed(const vec_t &u) {
			return std::sqrt(gamma * pressure(u) / u[0]);
		}

        inline static scalar_t sound_speed(const scalar_t density, const scalar_t pressure) {
			return std::sqrt(gamma * pressure / density);
		}

        /*
            compute wave speed
        */
        inline static scalar_t max_wave_speed(const vec_t &p, const size_t dim) {
            return p[1 + dim] + sound_speed(p);
        }

        inline static scalar_t min_wave_speed(const vec_t &p, const size_t dim) {
            return p[1 + dim] - sound_speed(p);
        }


    };
}
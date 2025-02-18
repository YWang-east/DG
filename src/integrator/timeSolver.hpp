#pragma once

#include "core/types.hpp"
#include <DG.hpp>

namespace DG::integrator::time {
    /**
     * @brief Second-order Runge-Kutta method
     * 
     */
    struct RK2 {
        void advance(auto &integrator, scalar_t dt) {

            integrator.compute_flux();
            for (auto &cell : integrator.cells) {
                cell.u0 = cell.u;
                cell.u += integrator.dudt(cell) * dt * 0.5;
            }

            integrator.compute_flux();
            for (auto &cell : integrator.cells) {
                cell.u = cell.u0 + integrator.dudt(cell) * dt;
            }
        }
    };

    /**
     * @brief Strong stability preserving Third-order Runge-Kutta method
     * 
     */
    struct SSP_RK3 {
        void advance(auto &integrator, scalar_t dt) {
            integrator.compute_flux();
            for (auto &cell : integrator.cells) {
                cell.u0 = cell.u;
                cell.u += integrator.dudt(cell) * dt;
            }

            integrator.compute_flux();
            for (auto &cell : integrator.cells) {
                cell.u = cell.u0 * 0.75 + cell.u * 0.25 + integrator.dudt(cell) * dt * 0.25;
            }

            integrator.compute_flux();
            for (auto &cell : integrator.cells) {
                cell.u = (cell.u0 + cell.u * 2.0 + integrator.dudt(cell) * dt * 2.0) / 3.0;
            }
        }
    };

    /**
     * @brief Five stage low storage Fourth-order Runge-Kutta method
     */
    struct LS_RK4 {

    };
}
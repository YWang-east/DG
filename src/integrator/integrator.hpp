#pragma once

#include <DG.hpp>
#include <cmath>
#include <cstddef>
#include "core/basis.hpp"
#include "cell.hpp"
#include "core/types.hpp"
#include "mesh/mesh.hpp"
#include "scene/scene.hpp"

namespace DG::integrator {

    template <typename Model, typename fSolver>
    class Integrator {
    private:
        static constexpr size_t ND = Model::NumDims;

    public:
        scene::Scene<Model> scene;

        RefCell ref_cell;

        std::vector<mesh::Face<ND>> faces;
        std::vector<Cell<Model>> cells;

        const scalar_t Ncells;
        const scalar_t Nnodes;
        const scalar_t porder;

        scalar_t cfl;

        /*
            Integrator constructor
        */
        Integrator(mesh::Mesh<ND> &mesh, scene::Scene<Model> &scene, size_t porder, scalar_t cfl) 
            : scene(scene), ref_cell(porder + 1), faces(mesh.faces), Ncells(mesh.Ncells), Nnodes(porder + 1), porder(porder), cfl(cfl)
        {   
            std::cout << "Initializing the simulation ..." << std::endl;

            /*
                Initialize integrator cells
            */
            cells.reserve(Ncells);
            for (size_t i = 0; i < Ncells; i++) {
                // get information from the mesh
                cells.push_back(Cell<Model>(Nnodes));
                cells[i].detJ = mesh.cells[i].detJ;
                cells[i].size = mesh.cells[i].size;

                // loop over each quadrature point and assign initial conditions
                for (size_t j = 0; j < Nnodes; j++) {
                    cells[i].x[j] = mesh.cells[i].map(ref_cell.x[j]);
                    cells[i].p.fill_row(j, scene.initial_condition(cells[i].x[j]));
                    cells[i].u.fill_row(j, Model::PtoU(cells[i].p[j]));
                }
                cells[i].f_star[L] = 0.0;
                cells[i].f_star[R] = 0.0;
            }
        }

        scalar_t compute_dt_global(){
            scalar_t dt_global = std::numeric_limits<scalar_t>::max();
            for (auto &cell : cells) {
                scalar_t dt = cfl * Model::compute_dt(cell) / std::pow(porder - 1, 2);
                dt_global = std::min(dt_global, dt);
            }
            return dt_global;
        }

        void compute_flux() {
            for (auto &face : faces) {
                if (face.loc == FaceLocation::LEFT) {

                    auto &c = cells[face.ip];   // ip, im points to the same cell for boundary faces
                    vec_t ub = scene.boundary_conditions[LEFT]().boundary_value(c.u[0]);
                    c.f_star[L] = Model::Fu(ub, 0);

                } else if (face.loc == FaceLocation::RIGHT) {

                    auto &c = cells[face.im];   // ip, im points to the same cell for boundary faces
                    vec_t ub = scene.boundary_conditions[RIGHT]().boundary_value(c.u[Nnodes-1]);
                    c.f_star[R] = Model::Fu(ub, 0);

                } else {
                    auto &cp = cells[face.ip];  // cell in the +normal dir
                    auto &cm = cells[face.im];  // cell in the -normal dir

                    // TODO: should use quadrature in multi D for higher order
                    vec_t u_minus = cm.u[Nnodes - 1];
                    vec_t u_plus  = cp.u[0];

                    // TODO: higher dim
                    cm.f_star[R] = fSolver::flux(u_minus, u_plus, 0.0);
                    cp.f_star[L] = cm.f_star[R];
                }
            }
        }

        mat_t dudt(auto &cell) {
            mat_t F_star(Nnodes, Model::NumEqns); 
            mat_t F(Nnodes, Model::NumEqns); 
            mat_t dudt(Nnodes, Model::NumEqns);

            F_star = 0.0;
            F_star.fill_row(0, cell.f_star[L]);
            F_star.fill_row(Nnodes - 1, cell.f_star[R]);

            // TODO: higher dim
            for (size_t i = 0; i < Nnodes; i++) {
                F.fill_row(i, Model::Fu(cell.u[i], 0.0));
            }

            dudt  = ref_cell.MinvB * (F - F_star) - ref_cell.D * F;
            dudt /= cell.detJ;

            return dudt;
        }

        void update_primitive() {
            for (auto &cell : cells) {
                for (size_t i = 0; i < Nnodes; i++) {
                    cell.p.fill_row(i, Model::UtoP(cell.u[i]));
                }
            }
        }
    };
}
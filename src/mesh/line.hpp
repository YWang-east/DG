#pragma once

#include "core/types.hpp"
#include "mesh.hpp"
#include <cstddef>

namespace DG::mesh {
    /**
     *  @brief mesh with line cells (1D)
     */
    struct Line : public Mesh<1> {

        Line(size_t Ncells, scalar_t L) {

            this->Ncells = Ncells;
            this->Nfaces = Ncells + 1;
            this->cells.resize(Ncells);
            this->faces.resize(Nfaces);

            scalar_t dx = L / Ncells;

            // initialize cell vertices
            for (size_t i = 0; i < Ncells; i++) {
                this->cells[i].index = i;
                this->cells[i].vertices.resize(2);
                this->cells[i].vertices[0].x = {i * dx};
                this->cells[i].vertices[1].x = {(i + 1) * dx};

                this->cells[i].map = [i=i, dx=dx](arr_t<1> x_ref) {
                    return arr_t<1>{0.5 * ((2*i + 1) * dx + x_ref[0] * dx)};
                };
                this->cells[i].size = {dx};
                this->cells[i].detJ = 0.5 * dx;
            }

            // initialize cell interfaces
            for (size_t i = 0; i <= Ncells; i++) {
                this->faces[i].center = {i * dx};
                this->faces[i].area = 1.0;
                faces[i].n = {1.0};
                faces[i].loc = FaceLocation::INTER;
                if (i == 0) {
                    faces[i].ip = 0;
                    faces[i].im = Ncells - 1;
                } else if (i == Ncells) {
                    faces[i].ip = 0;
                    faces[i].im = Ncells - 1;
                } else {
                    faces[i].ip = i;
                    faces[i].im = i - 1;
                }
            }
        }

        size_t set_boundary() {
            this->faces[0].loc = FaceLocation::LEFT;
            this->faces[0].im  = 0;

            this->faces[this->Ncells].loc = FaceLocation::RIGHT;
            this->faces[this->Ncells].ip  = this->Ncells - 1;

            return 2;   // number of boundaries
        }

        ~Line() = default;
    };
}
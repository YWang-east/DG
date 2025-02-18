#pragma once

#include <cstddef>
#include <functional>
#include <DG.hpp>
#include "core/types.hpp"

namespace DG::mesh {

    /**
     *  @brief nodes of a cell
     */
    template<const size_t ND>
    struct Vertex {
        arr_t<ND> x;   // coordinates of the Vertex
    };

    /**
     *  @brief cell
     */
    template<const size_t ND>
    struct Cell {
        size_t index;
        arr_t<ND> size;
        std::vector<Vertex<ND>> vertices;           // vertices of the cell
        std::function<arr_t<ND>(arr_t<ND>)> map;    // mapping to the reference cell
        scalar_t detJ;                              // determinant of the Jacobian of the mapping

        void print(){
            std::cout << "Cell vertices:\n";
            for (size_t i = 0; i < vertices.size(); i++) {
                std::cout << vertices[i].x;
            }
        }
    };

    /**
     *  @brief element interfaces
     */
    template<const size_t ND>
    struct Face {
        arr_t<ND> center;
        arr_t<ND> n;    // face normal vector 
        size_t ip;      // cell in the +normal dir  
        size_t im;      // cell in the -normal dir

        scalar_t area;

        size_t loc;     // location of the face
    };

    /**
     *  @brief store information about a mesh
     */
    template<size_t ND>
    struct Mesh {
        size_t Ncells;      // number of cells 
        size_t Nfaces;      // number of faces
        std::vector<Face<ND>> faces; 
        std::vector<Cell<ND>> cells;

        virtual ~Mesh() = default;
    };
}
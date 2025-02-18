#pragma once

#include <functional>
#include <DG.hpp>
#include "boundary.hpp"
#include "core/types.hpp"

namespace DG::scene {
    /**
     *  @brief Set up the scene for the simulation
     * 
     *  @param Model model class
     */
    template<typename Model>
    class Scene {
    private:
        /*
            Initial condition function pointer
            (x) -> (var_t)
        */
        using ic_t = std::function<vec_t(const arr_t<Model::NumDims>&)>;

        /*
            Boundary condition function pointer
            TODO: could add input arguments to the function, e.g. (x, t) -> (Boundary<Model>)
        */
        using bc_t = std::function<Boundary<Model>()>;

    public:  
        ic_t initial_condition;
        std::vector<bc_t> boundary_conditions;

    };
}
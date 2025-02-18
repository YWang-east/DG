#pragma once

#include <DG.hpp>

namespace DG::model {
    /**
     * @brief Base model class
     *        Store common flags and variables
     * 
     * @param ND number of spatial dimensions
     */
    template<const size_t ND>
    class Model {
    public:
        static constexpr size_t NumDims = ND;
    };
}
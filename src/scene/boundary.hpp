#pragma once

#include <DG.hpp>

namespace DG::scene {

	/**
	 *	@brief Define the type of a boundary face
	 */
	template<typename Model>
	struct Boundary {
		// using var_t = typename Model::var_t;
		// using dim_t = arr_t<Model::NumDims>;
		
		enum class Type {
			Dirichlet,
			Neumann
		} type;

		vec_t value;	// TODO: can be optional
		
		/*
			Constructors used by the scene to set up boundary conditions
			return values are saved into scene::face_info
		*/
		static Boundary Dirichlet(vec_t value) {
			return {Type::Dirichlet, value};
		}

		static Boundary Neumann() {
			vec_t value(Model::NumEqns);
			return {Type::Neumann, value};
		}

		/*
			return boundary values
			Only for fixed inlet conditions, member value remain unchanged; otherwise, value is updated based on boundary cell
		*/
		vec_t boundary_value(const vec_t &cell_value) {

			if (type == Type::Dirichlet) {

				return value;

			} else if (type == Type::Neumann) {

				return cell_value;
			}

			assert(false && "Invalid boundary condition.");
			return value;
		}
	};
}
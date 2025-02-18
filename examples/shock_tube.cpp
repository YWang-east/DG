// common lib headers
#include <DG.hpp>
#include "scene/scene.hpp"
#include "integrator/integrator.hpp"
#include "integrator/fluxSolver.hpp"
#include "integrator/timeSolver.hpp"
#include "driver/driver.hpp"

// problem specific headers
#include "mesh/line.hpp"
#include "model/euler.hpp"

using namespace DG;

int main() {
    // ##################
	// # MODEL, SOLVER  #
	// ##################
    using model_t = model::Euler<1>;
    using fSolver = integrator::flux::LaxFriedrichs<model_t>; 
    using tSolver = integrator::time::SSP_RK3;

    // ##################
	// # NUMERICS       #
	// ##################
    size_t porder = 4;
    size_t mesh_size = 50;
    scalar_t domain_size = 1.0;
    scalar_t cfl = 0.5;

    scalar_t end_time = 8e-4;
    scalar_t write_interval = 8e-5;

    mesh::Line mesh(mesh_size, domain_size);    // generate mesh
    
    // ##################
	// # SCENE          #
	// ##################
    scene::Scene<model_t> scene;

    // set up initial condition
    scene.initial_condition = [&] (const arr_t<1>& x) {
        if (x[0] < 0.5) 
            return vec_t{2.0, 0.0, 2.0e5};
        else
            return vec_t{1.0, 0.0, 1.0e5};
    };

    // set up boundary conditions
    scene.boundary_conditions.resize(mesh.set_boundary());

    scene.boundary_conditions[LEFT] = [] () {
        return scene::Boundary<model_t>::Neumann();
    };

    scene.boundary_conditions[RIGHT] = [] () {
        return scene::Boundary<model_t>::Neumann();
    };

    // ##################
	// # RUN SIMULATION #
	// ##################
    integrator::Integrator<model_t, fSolver> integrator{mesh, scene, porder, cfl};
    driver::Driver driver(EXAMPLE_NAME, integrator, tSolver{});
    driver.run(write_interval, end_time);
    
    return 0;
}
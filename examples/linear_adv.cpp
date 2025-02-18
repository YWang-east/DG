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
    size_t porder = 8;
    size_t mesh_size = 10;
    scalar_t domain_size = 1.0;
    scalar_t cfl = 0.5;

    scalar_t end_time = 1.0;
    scalar_t write_interval = 0.1;

    mesh::Line mesh(mesh_size, domain_size);    // generate mesh
    
    // ##################
	// # SCENE          #
	// ##################
    scene::Scene<model_t> scene;

    // set up initial condition
    scalar_t u_ref = 1.0;
    scene.initial_condition = [&] (const arr_t<1>& x) {
        scalar_t rho = 2.0 + std::sin(2.0 * M_PI * x[0]);
        return vec_t{rho, u_ref, 1.0};
    };

    // ##################
	// # RUN SIMULATION #
	// ##################
    integrator::Integrator<model_t, fSolver> integrator{mesh, scene, porder, cfl};
    driver::Driver driver(EXAMPLE_NAME, integrator, tSolver{});
    driver.run(write_interval, end_time);
    
    return 0;
}
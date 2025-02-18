#pragma once

#include "core/types.hpp"
#include <DG.hpp>
#include <fstream>
#include <string>

namespace DG::driver {

    template<typename Integrator, typename tSolver>
    class Driver {
    private:

        std::string simulation_name;
        Integrator integrator;
        tSolver timeSolver;

    public:

        Driver(std::string simulation_name, Integrator integrator, tSolver timeSolver) 
            : simulation_name(simulation_name), integrator(integrator), timeSolver(timeSolver) {}
        
        void run(scalar_t write_interval, scalar_t end_time) {
            scalar_t time = 0.0;
            scalar_t last_write_time = 0.0;
            size_t count = 0;

            // write initial condition;
            write_data(count);

            std::cout << "--------------------------------" << std::endl;
            std::cout << "Simulation starts" << std::endl;
            std::cout << "--------------------------------" << std::endl;

            while(time < end_time) {
                scalar_t dt = integrator.compute_dt_global();
                if (time + dt > end_time) {
                    dt = end_time - time;
                }
                time += dt;

                timeSolver.advance(integrator, dt);
                
                // write data
                if (time - last_write_time >= write_interval || time >= end_time) {
                    count++;
                    std::cout << "Frame " << count << " at time = " << time << std::endl;
                    last_write_time = time;
                    write_data(count);
                }
            }
        }

        // TODO: make it more general
        void write_data(size_t count) {
            std::ofstream file;
            file.open(simulation_name + "_" + std::to_string(count) + ".csv");

            integrator.update_primitive();

            // write header
            file << "x,rho,u,p" << std::endl;

            for (size_t i = 0; i < integrator.Ncells; i++) {
                for (size_t j = 0; j < integrator.Nnodes; j++) {
                    file 
                    << integrator.cells[i].x[j][0] << "," 
                    << integrator.cells[i].p[j][0] << "," 
                    << integrator.cells[i].p[j][1] << "," 
                    << integrator.cells[i].p[j][2] << "\n";
                }
            }

            file.close();
        }
    };
}
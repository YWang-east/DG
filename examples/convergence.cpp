#include <DG.hpp>
#include <cstddef>
#include <fstream>
#include "core/basis.hpp"
#include "core/types.hpp"

using namespace DG;

inline vec_t f(vec_t &x) {
    vec_t result(x.size());
    for (size_t i = 0; i < x.size(); i++) {
        result[i] = std::exp(std::sin(M_PI * x[i]));
    }
    return result;
}

inline vec_t dfdx(vec_t &x) {
    vec_t result(x.size());
    for (size_t i = 0; i < x.size(); i++) {
        result[i] = M_PI * std::cos(M_PI * x[i]) * std::exp(std::sin(M_PI * x[i]));
    }
    return result;
}

int main() {

    size_t porder_max = 24;
    size_t n_cell_max = 8;

    std::ofstream file1, file2;

    // ##################
	// # p-adaptivity   #
	// ##################
    file1.open("p_adaptivity.csv");
    file1 << "p,error" << std::endl;

    for (size_t p = 2; p <= porder_max; p++) {
        RefCell ref_cell(p + 1);
        mat_t D = ref_cell.D;           // differentiation matrix
        vec_t x = ref_cell.x;           // quadrature points

        vec_t dfdx_approx = D * f(x);

        vec_t eps = dfdx_approx - dfdx(x);

        // compute L2 error
        scalar_t error = 0.0;
        for (size_t i = 0; i < eps.size(); i++) {
            error += std::pow(eps[i], 2);
        }
        error = std::sqrt(error / eps.size());

        file1 << p << "," << error << std::endl;
    }

    file1.close();

    // ##################
	// # h-adaptivity   #
	// ##################
    file2.open("h_adaptivity.csv");
    file2 << "n,error" << std::endl;

    size_t order = 2;
    size_t n_nodes = order + 1;
    RefCell ref_cell(n_nodes);
    mat_t D = ref_cell.D;  

    for (size_t n = 1; n <= n_cell_max; n++) {

        vec_t dfdx_approx(n_nodes * n);
        vec_t eps(n_nodes * n);

        // loop over cells
        for (size_t i = 0; i < n; i++) {
            scalar_t xl = -1.0 + 2.0 * i / n;
            scalar_t xr = -1.0 + 2.0 * (i + 1) / n;
            scalar_t dx = (xr - xl);

            vec_t x = ref_cell.x;
            for (size_t j = 0; j < x.size(); j++) {
                x[j] = xl + 0.5 * (1 + x[j]) * dx;
            }

            vec_t dfdx_approx = D * f(x) / (0.5 * dx);
            vec_t dfdx_exact  = dfdx(x);

            for (size_t j = 0; j < x.size(); j++) {
                eps[i * x.size() + j] = dfdx_approx[j] - dfdx_exact[j];
            }
        }

        // compute L2 error
        scalar_t error = 0.0;
        for (size_t i = 0; i < eps.size(); i++) {
            error += std::pow(eps[i], 2);
        }
        error = std::sqrt(error / eps.size());

        file2 << n_nodes * n << "," << error << std::endl;
    }

    file2.close();
    
    return 0;
}
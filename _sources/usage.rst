Usage
=====

Vectors and matrices
--------------------

Basics
~~~~~~

To declare DG vectors and matrices, you can do the following:

.. code-block:: c++

    #include <DG.hpp>

    using namespace DG;

    int main() {
        vec<double> v0(3);          // define a vector of size 3
        matrix<double> M0(3, 3);    // define a matrix of size 3x3
    }

For convenience, you can use ``vec_t`` and ``mat_t`` for ``vec<double>`` and ``matrix<double>`` respectively. 

Initialize with elements:

.. code-block:: c++

    vec<int> v1 = {1, 2, 3};
    matrix<int> M1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

Print:

.. code-block:: c++

    std::cout << v1 << std::endl;
    std::cout << M1 << std::endl;

Then you will get the following printed in your terminal:

.. code-block:: console

    vector(3):
    [1, 2, 3]

    matrix(3, 3):
    [1, 2, 3]
    [4, 5, 6]
    [7, 8, 9]

Access and change elements:

.. code-block:: c++

    v1[1] = 10;
    M1(2, 1) = 10;

    std::cout << v1 << std::endl;
    std::cout << M1 << std::endl;

    std::cout << M1[1] << std::endl;    // get the second row
    M1.fill_row(1, v1);                 // change the second row with v1
    std::cout << M1 << std::endl;

Output

.. code-block:: console

    vector(3):
    [1, 10, 3]

    matrix(3, 3):
    [1, 2, 3]
    [4, 5, 6]
    [7, 10, 9]

    vector(3):
    [4, 5, 6]

    matrix(3, 3):
    [1, 2, 3]
    [1, 10, 3]
    [7, 10, 9]


Operations
~~~~~~~~~~

You can do basic operations on vectors and matrices:

.. code-block:: c++

    vec_t v2 = v1 * 2.0;  // multiply or divide by a scalar
    vec_t v3 = v1 + v2;   // add or subtract vectors
    mat_t M2 = M1 * 2.0;  // same for matrices

    double norm = v2.norm();        // get the norm of a vector

In addition, the code provide linear algebra operations:

.. code-block:: c++

    vec_t v4 = M1 * v1;         // matrix-vector multiplication
    mat_t M3 = M1 * M2;         // matrix-matrix multiplication
    mat_t M4 = M1.transpose();  // matrix transpose
    mat_t M5 = M1.inv();        // matrix inverse


Quadrature
----------

Initialize Gaussian-Lobatto quadrature with 5 points:

.. code-block:: c++

    #include <DG.hpp>
    #include "core/quadrature.hpp"

    using namespace DG;

    int main() {
        Lobatto Q(5);
        std::cout << Q.x << std::endl;  // quadrature points
        std::cout << Q.w << std::endl;  // weights
    }

Output:

.. code-block:: console

    vector(5):
    [-1, -0.654654, 0, 0.654654, 1]

    vector(5):
    [0.1, 0.544444, 0.711111, 0.544444, 0.1]

Initialize a reference cell with 5 Gaussian-Lobatto quadrature points:

.. code-block:: c++

    RefCell C(5);
    std::cout << C.M << std::endl;  // mass matrix
    std::cout << C.D << std::endl;  // differentiation matrix

Output:

.. code-block:: console

    matrix(5, 5):
    [0.0888889, 0.0259259, -0.0296296, 0.0259259, -0.0111111]
    [0.0259259, 0.483951, 0.0691358, -0.0604938, 0.0259259]
    [-0.0296296, 0.0691358, 0.632099, 0.0691358, -0.0296296]
    [0.0259259, -0.0604938, 0.0691358, 0.483951, 0.0259259]
    [-0.0111111, 0.0259259, -0.0296296, 0.0259259, 0.0888889]

    matrix(5, 5):
    [-5, 6.7565, -2.66667, 1.41016, -0.5]
    [-1.24099, 4.22463e-16, 1.74574, -0.763763, 0.25901]
    [0.375, -1.33658, 1.08607e-16, 1.33658, -0.375]
    [-0.25901, 0.763763, -1.74574, -5.33486e-16, 1.24099]
    [0.5, -1.41016, 2.66667, -6.7565, 5]


Models and solvers
------------------

The model class takes in a constant template argument that is the dimension. All its members are ``static``. We recommend create a type alias and access all model class methods with ``::``. For example, initialize 1D Euler equations:

.. code-block:: c++

    #include <DG.hpp>
    #include "model/euler.hpp"  // include the corresponding model header

    using namespace DG;

    int main() {
        using euler_1D = model::Euler<1>;

        // primitive variables: rho, u, p
        vec_t P = {1.0, 1.0, 1.0};

        // conserved variables: rho, rho*u, rho*E
        vec_t U = euler_1D::PtoU(P);

        // compute flux vector using conserved variables
        vec_t F = euler_1D::Fu(U);
    }

Similarly, you can define flux solvers and time integration methods as

.. code-block:: c++

    // additional headers
    #include "integrator/fluxSolver.hpp"
    #include "integrator/timeSolver.hpp"

    ...

        using fSolver = integrator::flux::LaxFriedrichs<euler_1D>;
        using tSolver = integrator::time::RK2;

Initial and Boundary Conditions
-------------------------------

Initial and boundary conditions are defined via the ``scene``, which later parses them to the computation. To initialize a scene, you need to provide the template argument that is the model type, e.g.,

.. code-block:: c++

    #include "model/euler.hpp"
    #include "scene/scene.hpp"

    using namespace DG;

    int main() {
        using euler_1D = model::Euler<1>;
        scene::Scene<euler_1D> scene;
    }

To set the initial condition, do need to define a lambda function that takes in the coordinates ``x`` and return the primitive variables of the model:

.. code-block:: c++

        scene.initial_condition = [](const arr_t<1>& x) -> vec_t {
            // your initial condition goes here
        };

If no boundary conditions are specified, the default ones are periodic. Please refer to ``examples/shock_tube.cpp`` to see how to set up boundary conditions.


Run the Simulation
------------------

Please refer to ``examples/shock_tube.cpp``

Data output
-----------

Simulation results are in ``.csv`` format. They are located in the same directory as the executable.



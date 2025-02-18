Architecture
============

Goal
----

The goal of this code is to solve hyperbolic conservation laws on unstructured meshes using the Discontinuous Galerkin (DG) method. 

Design Philosophy
-----------------

The code is designed to be flexible and modular such that users are free to specify their own physical problems and choose the relevant numerical solvers. It consists of two main components:

- ``DG`` library, which contains all functionalities of the code, and
- ``examples``, where users can define their physical problems and use the ``DG`` library to run the simulations.

DG library
----------

The below table illustrates the components of the library:

+---------------------------+--------------------------------------------------+
| Modules                   | Functionality                                    |
+===========================+==================================================+
| ``DG::core``              | Data types, quadrature, operation matrices       |
+---------------------------+--------------------------------------------------+
| ``DG::driver``            | Run the simulation and write results             |
+---------------------------+--------------------------------------------------+
| ``DG::integrator``        | Flux and time solvers, actual computation        |
+---------------------------+--------------------------------------------------+
| ``DG::mesh``              | Generate mesh                                    |
+---------------------------+--------------------------------------------------+
| ``DG::model``             | Physical models                                  |
+---------------------------+--------------------------------------------------+
| ``DG::scene``             | Parse simulation set-up to the integrator        |
+---------------------------+--------------------------------------------------+

Data structure
--------------

``DG::core::vec<typename T>``

A class which encapsulates a ``std::vector`` and has various convenient array operations. Alias ``vec_t`` is commonly used in the code.

``DG::core::matrix<typename T>``

A derived class based on `vec` and has basic matrix operations: transpose, inverse, multiplication etc. Alias ``mat_t`` is commonly used in the code.

``DG::integrator::Cell<typename Model>``

A class that stores solutions, used for the computation.

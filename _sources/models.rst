Physical Models
===============

The code solves PDEs in the form

.. math::

   \frac{\partial \boldsymbol{u}}{\partial t} + \nabla \cdot \boldsymbol{F} 
   = \boldsymbol{0}.

The Euler Equations
-------------------

One example of a hyperbolic conservation law is the Euler equations of gas dynamics. In 1D, they read

.. math::

   \frac{\partial }{\partial t} \left( \begin{array}{c} \rho \\ \rho u \\ E \end{array} \right) + 
   \frac{\partial }{\partial x} \left( \begin{array}{c} \rho u \\ \rho u^2 + p \\ u(E + p) \end{array} \right) 
   = 0.
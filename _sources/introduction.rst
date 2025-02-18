Introduction to DG
==================

The Discontinuous Galerkin Method
---------------------------------

Consider the 1D scalar conservation law

.. math:: 
   
      \frac{\partial u}{\partial t} + \frac{\partial f(u)}{\partial x} = 0.

To solve the above equation, the idea is to discretize the physical domain into `K` non-overlapping cells :math:`D^k`. The local solution within the cell is assumed to be

.. math:: 

   x \in D^k=[x_l^k, \ x_r^k]: \ u_h^k(x) = \sum_{i=1}^{N_p} u_h^k(x_i^k) \ell_i^k(x),

where :math:`x_i^k` are the :math:`N_p` distinct nodes (or grid points) and :math:`\ell_i^k(x)` are polynomials of degree :math:`N_p - 1`.

Multiplying the conservation law by a test function :math:`\ell_j^k(x)` and integrating over the cell :math:`D^k` lead to

.. math:: 

   \mathcal{M}^k \frac{d}{d t} \boldsymbol{u}_h^k+\mathcal{S}^k\boldsymbol{f}_h^k =\left[\ell^k(x) \left(f_h^k - f_h^*\right)\right]_{x_l^k}^{x_r^k},

with

- the solution vector :math:`\boldsymbol{u}_h^k = [u_1^k, \dots, u_{N_p}^k]^T`,
- the flux vector :math:`\boldsymbol{f}_h^k = [f_1^k, \dots, f_{N_p}^k]^T`,
- the numerical flux :math:`f_h^* = f(u_h^*)`,
- the mass and stiffness matrices

.. math::

   \mathcal{M}^k = \int_{D^k} \ell_i^k(x) \ell_j^k(x) dx, \quad 
   \mathcal{S}^k = \int_{D^k} \ell_i^k(x) \frac{d \ell_j^k(x)}{dx} dx. 

Unlike the conventional finite element method (FEM), solutions can be discontinuous at cell interfaces. They are coupled through the numerical flux :math:`f_h^*`, which recovers the global solution. This property resembles the finite volume method (FVM), but the DG method can easily achieve high-order accuracy by simply increasing the local polynomial degree.

Comparison with FDM, FVM, and FEM
---------------------------------

+---------+--------------------+-----------------+---------------------+-------------------+
| Methods | Complex geometries | `hp`-adaptivity | Hyperbolic problems | Elliptic problems |
+=========+====================+=================+=====================+===================+
| FDM     | X                  | √               | √                   | √                 |
+---------+--------------------+-----------------+---------------------+-------------------+
| FVM     | √                  | X               | √                   | (√)               |
+---------+--------------------+-----------------+---------------------+-------------------+
| FEM     | √                  | √               | X                   | √                 |
+---------+--------------------+-----------------+---------------------+-------------------+
| DG-FEM  | √                  | √               | √                   | (√)               |
+---------+--------------------+-----------------+---------------------+-------------------+


Legendre-Gauss-Lobatto quadrature
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Consider a reference cell :math:`D^{ref} = [-1, \ 1]`, where the local solution can be approximated by

.. math::

   r\in D^{ref}: \  u_h(r) = \sum_{i=1}^{N_p} u(r_i) \ell_i(r),

with the Lagrange polynomial basis

.. math::

   \ell_i(r) = \prod_{\substack{j=1 \\ j \neq i}}^{N_p} \frac{r-r_j}{r_i-r_j}.

Here, :math:`r_i` are the Legendre-Gauss-Lobatto quadrature nodes. 

The mass and stiffness matrices in the reference cell can be computed as

.. math::

   \mathcal{M} = (\mathcal{V} \mathcal{V}^T)^{-1}, \quad \mathcal{S} = \mathcal{M} \mathcal{D},

where the Vandermonde matrix :math:`\mathcal{V}` and the differentiation matrix :math:`\mathcal{D}` are

.. math::

   \mathcal{V}_{ij} = \tilde{P}_j(r_i), \quad 
   \mathcal{V}_{r, (ij)} = \frac{d \tilde{P}_j}{d r} \Bigg|_{r_i}, \quad
   \mathcal{D} = \mathcal{V}_r \mathcal{V}^{-1}

with :math:`\tilde{P}_j(r)` the Legendre polynomial.

Cell-wise operations
~~~~~~~~~~~~~~~~~~~~

The reference cell can be mapped to a generic cell :math:`D^k = [x_l^k, \ x_r^k]` through the mapping

.. math::

   x(r) = x_l^k + \frac{1+r}{2}(x_r^k - x_l^k).

The mass and stiffness matrices after the mapping become

.. math::

   \mathcal{M}^k = \frac{\Delta x}{2} \mathcal{M}, \quad 
   \mathcal{S}^k = \mathcal{M} \mathcal{D}

Finally the DG scheme can be written as

.. math::

   \frac{d \boldsymbol{u}^k}{dt} = \frac{2}{\Delta x} \left(
    - \mathcal{D} \boldsymbol{f}^k 
    + \mathcal{M}^{-1} \mathcal{B} (\boldsymbol{f}^k - \boldsymbol{f}^*) \right)

with boundary matrix :math:`\mathcal{B} = \textrm{diag}(-1,0,\dots,0,1)`

Numerical flux
-------------- 

The implemented numerical flux solvers are

Lax-Friedrichs flux
~~~~~~~~~~~~~~~~~~~

.. math::

    f^* = \frac{f(u^+) + f(u^-)}{2} - \frac{\alpha}{2}(u^+ - u^-),

where :math:`u^+` and :math:`u^-` are the right and left nodal values across a cell interface respectively, and

.. math::
   
   \alpha = \max |f'(u)|.


Time Integration
----------------

Consider the semi-discrete equation

.. math::

    \frac{du}{dt} = L(u),


where operator :math::`L` stands for the resultant spatial discretization from the DG method. Currently, The implemented time integration methods are

2nd order Runge-Kutta (``RK2``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. math::

   u^{(1)} = u^n +  \frac{\Delta t}{2} L(u^n) \\
   u^{n+1} = u^{(1)} + \Delta t L(u^{(1)})


Strong stability preserving 3rd order Runge-Kutta (``SSP_RK3``)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. math::

   u^{(1)} &= u^n +  \Delta t L(u^n) \\
   u^{(2)} &= \frac{3}{4}u^n + \frac{1}{4}u^{(1)} + \frac{\Delta t}{4} L(u^{(1)}) \\
   u^{n+1} &= \frac{1}{3}u^n + \frac{2}{3}u^{(2)} + \frac{2\Delta t}{3} L(u^{(2)}) 


.. admonition:: Reference
   :class: note

   [1] J. S. Hesthaven and T. Warburton (2008). Nodal Discontinuous Galerkin Methods. Springer.



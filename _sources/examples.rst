Examples
========

Derivative approximation
------------------------

In this example (``examples/convergence.cpp``), the accuracy of derivative approximation using the differentiation matrix :math:`\mathcal{D}` is demonstrated. Consider the following function:

.. math::

    f(x) = \exp(\sin(\pi x)) \quad \text{for} \ \ x \in [-1, 1]

The derivative of :math:`f(x)` is given by the exact expression

.. math::

    f'(x) = \pi \cos(\pi x) \exp(\sin(\pi x)),

or by the approximation

.. math::

    \mathcal{D} \cdot \boldsymbol{f}_h.

The L2-error of the approximation

.. math::

    || f'(x) - \mathcal{D} \cdot \boldsymbol{f}_h ||_{\Omega},

is examined when increasing the degree of freedom, which can be done either by increasing the local polynomial order (:math:`p` - adaptivity) or the number of cells (:math:`h` - adaptivity). The results plotted in the figure show the advantage of using high order local polynomial.

.. figure:: figures/convergence.pdf
    :alt: Summary Figure
    :align: center
    :figwidth: 80%

    Spectral convergence. In `p` - adaptivity, cell number is fixed as 1; In `h` - adaptivity: polynomial order is fixed as 3.

Sod's shock tube problem
-------------------------

In this example (``examples/shock_tube.cpp``), the solution to the Euler equations using the DG method is demonstrated. Suppose we have a domain of size :math:`L=1`, the initial conditions are given by

.. math::

    (\rho, \ u, \ p) 
    = \left\{ 
        \begin{array}{ll} 
            (2, \ 0, \ 2e5) \ \mathrm{ \ \ if \ \ } x<0.5, \\ 
            (1, \ 0, \ 1e5) \ \mathrm{ \ \ else. \ \ }
        \end{array} 
    \right.

The simulation uses a polynomial order of 4 and total 50 cells. Flux and time solver are chosen as ``LaxFriedrichs`` and ``SSP_RK3`` respectively. The results at time :math:`t=8e^{-4}` are shown in the figures below.

.. figure:: figures/shock_tube_rho.pdf
    :alt: Summary Figure
    :align: center
    :figwidth: 80%

    Density

.. figure:: figures/shock_tube_u.pdf
    :alt: Summary Figure
    :align: center
    :figwidth: 80%
    
    Velocity

.. figure:: figures/shock_tube_p.pdf
    :alt: Summary Figure
    :align: center
    :figwidth: 80%
    
    Pressure

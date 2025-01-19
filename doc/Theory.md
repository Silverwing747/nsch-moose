Model Description
=====

There are many Navier-Stokes/Cahn-Hilliard equations. In this project, we choose the one developed in [Dong, 2012](https://doi.org/10.1016/j.jcp.2012.04.041). Let $\Omega \subset \mathbb{R}^d$ ($d=$ 2 or 3) denote the flow domain, and $\Gamma = \partial \Omega$ denote the boundary of $\Omega$. Consider a mixture of two immiscible, incompressible fluids contained in $\Omega$. Let $\rho_1$ and $\rho_2$ respectively denote the densities of the two fluids, and $\mu_1$ and $\mu_2$ denote their dynamic viscosities. With the phase field approach, this two-phase system is described by the following coupled system of equations:

$$
\begin{aligned}
    &\rho \left( \frac{\partial \mathbf{u}}{\partial t} + \mathbf{u} \cdot \nabla \mathbf{u}\right) = \nabla \cdot p\mathbf{I} + \nabla \cdot [\mu(\nabla \mathbf{u} + \nabla \mathbf{u}^T)] - \lambda \nabla \cdot (\nabla \phi \otimes \nabla \phi) + \rho\mathbf{g}, \\
    &\nabla \cdot \mathbf{u} = 0, \\
    &\frac{\partial \phi}{\partial t} + \mathbf{u} \cdot \nabla \phi = -\nabla \cdot (M \lambda \nabla \mu_c), \\
    &\mu_c = \nabla ^2 \phi - h(\phi).
\end{aligned}
$$

In the above equations, $\mathbf{u}(\mathbf{x},t)$ is velocity, $p(\mathbf{x},t)$ is pressure, and $\mathbf{g}(\mathbf{x},t)$ is gravity, where $t$ is time and $\mathbf{x}$ is the spatial coordinate. $\phi(\mathbf{x},t)$ denotes the phase field function, $-1 \leq \phi \leq 1$; the flow regions with $\phi = 1$ and $\phi = -1$ respectively represent the first and the second fluids, and the iso-surface $\phi (\mathbf{x},t)=0$ marks the interface between the two fluids at time $t$. The function $h(\phi)$ is given by $h(\phi)=\frac{1}{\eta^2}\phi (\phi^2-1)$ by assuming a double-well potential, where $\eta$ is a characteristic length scale of the interface thickness. $\lambda$ is the mixing energy density and is related to the surface tension by

$$
\lambda = \frac{3}{2\sqrt{2}} \sigma \eta,
$$

where $\sigma$ is the interfacial surface tension and $M$ is the mobility of the interface. In this project, we assume both $\sigma$ and $M$ are constants. The density, $\rho$, and the dynamic viscosity, $\mu$, are related to the phase field function by

$$
\rho(\phi) = \frac{\rho_1 + \rho_2}{2} + \frac{\rho_1-\rho_2}{2}\phi, \mu(\phi) = \frac{\mu_1 + \mu_2}{2} + \frac{\mu_1-\mu_2}{2} \phi.
$$

In the equations above, the first equation is the Navier-Stokes equation, in which the term $\lambda \nabla \cdot (\nabla \phi \otimes \nabla \phi)$ is an additional interfacial stress acting across the interface. The second equation is the incompressibility constraint. The third equation is the Cahn-Hilliard equation. For more details, we refer readers to [Ding, 2007](https://doi.org/10.1016/j.jcp.2007.06.028) and [Dong, 2012](https://doi.org/10.1016/j.jcp.2012.04.041).

# Numerical Implementation

We adopt the variational multiscale (VMS) method for the numerical implementation. The VMS method is a numerical technique used to solve partial differential equations, particularly in fluid dynamics. It involves decomposing the solution into coarse and fine scales, allowing for more accurate and efficient simulations of complex systems. By capturing the effects of smaller scales on the larger scales, VMS improves the stability and accuracy of the numerical solution, making it particularly useful for simulating turbulent flows and multiphase interactions. For the exact formulation, we refer readers to the book chapter ["Phase-Field Modeling for Flow Simulation"](https://doi.org/10.1007/978-3-031-36942-1_4) in *Frontiers in Computational Fluid-Structure Interaction and Flow Simulation*, T. E. Tezduyar, Ed. (Springer International Publishing, Cham, 2023).

We implemented the generalized - $\alpha$ method for time integration; see [GeneralizedAlpha.md](GeneralizedAlpha.md) for implementation details. 

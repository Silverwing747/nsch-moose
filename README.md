Navier-Stokes Cahn-Hilliard (NSCH) for MOOSE Framework
=====

The MOOSE Navier-Stokes Cahn-Hilliard (NSCH) application is a library for implementing simulation tools that solve the Navier-Stokes Cahn-Hilliard equations with non-matching densities using Galerkin finite element methods with a residual-based stabilization scheme.

# MOOSE Framework

The Multiphysics Object-Oriented Simulation Environment (MOOSE) is a finite-element, multiphysics framework primarily developed by [Idaho National Laboratory](https://inl.gov/). It provides a high-level interface to some of the most sophisticated nonlinear solver technology on the planet. MOOSE presents a straightforward API that aligns well with the real-world problems scientists and engineers need to tackle. Every detail about how an engineer interacts with MOOSE has been thought through, from the installation process through running your simulation on state of the art supercomputers, the MOOSE system will accelerate your research.

Some of the capabilities enabled by MOOSE:

- Fully-coupled, fully-implicit multiphysics solver
- Dimension independent physics
- Automatically parallel (largest runs >100,000 CPU cores!)
- Modular development simplifies code reuse
- Built-in mesh adaptivity
- Continuous and Discontinuous Galerkin (DG) (at the same time!)
- Intuitive parallel multiscale solves (see videos below)
- Dimension agnostic, parallel geometric search (for contact-related applications)
- Flexible, pluggable graphical user interface
- Approximately 30 pluggable interfaces allow specialization of every part of the solve

For more information, including installation instructions, please see the official [MOOSE website](https://mooseframework.inl.gov).

# The Navier-Stokes Cahn-Hilliard (NSCH) Equations

The Navier-Stokes Cahn-Hilliard (NSCH) equations describe the behavior of two-phase fluid flow with non-matching densities. These equations combine the Navier-Stokes equations, which govern the motion of viscous fluid substances, with the Cahn-Hilliard equation, which models phase separation and interfacial dynamics. The NSCH equations are particularly useful for simulating complex fluid interactions where the densities of the two phases are different, such as in multiphase flows with oil and water. The equations account for the conservation of mass, momentum, and the phase field variable, providing a comprehensive framework for studying the dynamics of immiscible fluids.

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

We adopt the variational multiscale (VMS) method for the numerical implementation. The VMS method is a numerical technique used to solve partial differential equations, particularly in fluid dynamics. It involves decomposing the solution into coarse and fine scales, allowing for more accurate and efficient simulations of complex systems. By capturing the effects of smaller scales on the larger scales, VMS improves the stability and accuracy of the numerical solution, making it particularly useful for simulating turbulent flows and multiphase interactions. For the exact formulation, we refer readers to the book chapter ["Phase-Field Modeling for Flow Simulation"](https://doi.org/10.1007/978-3-031-36942-1_4) in *Frontiers in Computational Fluid-Structure Interaction and Flow Simulation*, T. E. Tezduyar, Ed. (Springer International Publishing, Cham, 2023). We use the genearlized-$\alpha$ method for time integration; see [GeneralizedAlpha.md](doc/GeneralizedAlpha.md) for implementation details. 







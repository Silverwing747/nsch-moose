Navier-Stokes Cahn-Hilliard (NSCH) for MOOSE Framework
=====

The MOOSE Navier-Stokes Cahn-Hilliard (NSCH) application is a library for implementing simulation tools that solve the Navier-Stokes Cahn-Hilliard equations with non-matching densities using Galerkin finite element methods with a residual-based stabilization scheme.

## Navier-Stokes Cahn-Hilliard (NSCH) equations

The Navier-Stokes Cahn-Hilliard (NSCH) equations describe the behavior of two-phase fluid flow with non-matching densities. These equations combine the Navier-Stokes equations, which govern the motion of viscous fluid substances, with the Cahn-Hilliard equation, which models phase separation and interfacial dynamics. The NSCH equations are particularly useful for simulating complex fluid interactions where the densities of the two phases are different, such as in multiphase flows with oil and water. The equations account for the conservation of mass, momentum, and the phase field variable, providing a comprehensive framework for studying the dynamics of immiscible fluids. 

For more information, please see the [theory page](doc/Theory.md) 

## MOOSE Framework

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

For more information, please see the official [MOOSE website](https://mooseframework.inl.gov).

# Tutorial

## Install MOOSE on HPC Cluster
1. Load required modules on the cluster:

    ```bash
    module load gcc/10.2.0 openmpi/4.0.5-gcc10.2.0
    module load anaconda3/2020.11
    ```

2. Cloning MOOSE

    MOOSE is hosted on GitHub and should be cloned directly from there using git. We recommend creating a projects directory to contain all of your MOOSE related work. To clone MOOSE, run the following commands in a terminal:

    ```bash
    mkdir -p ~/projects
    cd ~/projects
    git clone https://github.com/idaholab/moose.git
    cd moose
    git checkout master
    ```

3. 




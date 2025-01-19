# Generalized $\alpha$ Method

The Generalized Alpha Method is a numerical integration technique used for solving differential equations, particularly in the context of structural dynamics and transient analysis. It is designed to provide a balance between accuracy and numerical dissipation, allowing for the control of high-frequency response damping while maintaining second-order accuracy for low-frequency response.

Key features of the Generalized Alpha Method include:
- Control over numerical dissipation through parameters that can be adjusted to achieve desired damping characteristics.
- Second-order accuracy for both displacement and velocity.
- Stability for a wide range of time step sizes.

This method is particularly useful in simulations where controlling the high-frequency response is crucial, such as in the analysis of structural vibrations and wave propagation problems.

## Theory

The algorithm can be formulated as follows: Given $\dot{V}_n$, $V_n$, and $\Delta t_n = t_{n+1} - t_n$, find $\dot{V}_{n+1}$, $V_{n+1}$, $\dot{V}_{n+\alpha_m}$, $V_{n+\alpha_f}$ such that the residual vector:

$$
R(\dot{V}_{n+\alpha_m}, V_{n+\alpha_f}) = 0,
$$

where:

$$
V_{n+1} = V_n + \Delta t_n \dot{V}_n + \gamma \Delta t_n (\dot{V}_{n+1} - \dot{V}_n),
$$

$$
\dot{V}_{n+\alpha_m} = \dot{V}_n + \alpha_m (\dot{V}_{n+1} - \dot{V}_n),
$$

$$
V_{n+\alpha_f} = V_n + \alpha_f (V_{n+1} - V_n),
$$

and $\alpha_m$, $\alpha_f$, and $\gamma$ are parameters of the generalized-$\alpha$ method.

Second-order accuracy in time is achieved if we choose:

$$
\gamma = \frac{1}{2} + \alpha_m - \alpha_f,
$$

whereas $A$-stability can be achieved with:

$$
\alpha_m \geq \alpha_f \geq \frac{1}{2}.
$$

The parameters $\alpha_m$ and $\alpha_f$ can be expressed in terms of $\rho_\infty$, which is the spectral radius of the amplification matrix as $\Delta t_n \to \infty$. Using $\rho_\infty$, we define the values of $\alpha_m$ and $\alpha_f$ as follows:

$$
\alpha_m = \frac{1}{2} \left( \frac{3 - \rho_\infty}{1 + \rho_\infty} \right), \quad \alpha_f = \frac{1}{1 + \rho_\infty}.
$$

Taking $\rho_\infty \in [0,1]$ and using the above equations to determine $\alpha_m$ and $\alpha_f$, the generalized-$\alpha$ method is second-order accurate and $A$-stable. Throughout this work, we have chosen:

$$
\rho_\infty = \frac{1}{2}.
$$
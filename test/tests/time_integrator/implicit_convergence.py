#!/usr/bin/env python3

import mms
df1 = mms.run_temporal('implicit_convergence.i', 4, 'Executioner/TimeIntegrator/rho_infty=1.0', console=False, dt=0.0625, y_pp='l2_err', executable='../../..')
df2 = mms.run_temporal('implicit_convergence.i', 4, 'Executioner/TimeIntegrator/rho_infty=0.5', console=False,
                       dt=0.0625, y_pp='l2_err', executable='../../..')
df3 = mms.run_temporal('implicit_convergence.i', 4, 'Executioner/TimeIntegrator/rho_infty=0.0', console=False,
                       dt=0.0625, y_pp='l2_err', executable='../../..')
fig = mms.ConvergencePlot(xlabel=r'$\Delta$t', ylabel='$L_2$ Error')
fig.plot(df1, label=r'$\rho_\infty$ = 1.0', marker='o', markersize=8)
fig.plot(df2, label=r'$\rho_\infty$ = 0.5', marker='o', markersize=8)
fig.plot(df3, label=r'$\rho_\infty$ = 0.0', marker='o', markersize=8)
fig.save('implicit_convergence.png')
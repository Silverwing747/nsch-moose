[GlobalParams]
  L0 = 1e-4
  gravity = 0.0
  Ca = 0.02
  Mobility = 1e-7
  MonotonicPressure = true 
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
  []
[]

[Variables]
    [./u]
      family = LAGRANGE_VEC
    [../]
    [./p]
    [../]
    [./phi]
    [../]
    [./mu]
    [../]
[]

[Kernels]
  [./ResMass]
    type = ADResMass
    variable = p      
  []
  [./ResMom]
    type = ADResMom
    variable = u
  []
  [./ResPhi]
    type = ADResPhi
    variable = phi      
  []
  [./ResMu]
    type = ADResMu
    variable = mu
  []
[]

[ICs]
  [./phi_ic]
    type = FunctionIC
    variable = phi
    function = 'tanh(sqrt(2.0)/2.0 * ((sqrt((x-0.5001)^2 + (y-0.5001)^2) - 0.25) / (0.02)))'
    #function = '-tanh(sqrt(2)/2.0 * ((sqrt((x-0.5001)^2 + (0.5001-0.5001)^2) - 0.25) / (0.02)))'
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'mu phi u p'
      auto_direction = 'x y'
    [../]
  [../]
[]

# [BCs]
#   [./no_slip]
#     type = VectorFunctionDirichletBC
#     variable = u
#     boundary = 'bottom right left top'
#   [../]
# []

[Materials]
  [./ADMaterial_NSCH]
    type = ADMaterial_NSCH
    phase_field = phi
    chemical_potential = mu
    velocity = u
    pressure = p
    output_properties = 'rho ResMomStrong'
    outputs = Soln
  [../]
[]

[Preconditioning]
  active = 'SMP'
  [./MUMPS]
    type = SMP
    full = true

    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_type'
    petsc_options_value = 'preonly   lu      mumps'
  []
  [./SMP]
    type = SMP
    full = true

    petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
    petsc_options_value = 'asm      50                 preonly       lu           0'

    # petsc_options_iname = '-pc_type -pc_hypre_type -ksp_gmres_restart -pc_hypre_boomeramg_strong_threshold'
    # petsc_options_value = 'hypre    boomeramg      31                 0.7'

  [../]
  [./FDP_n_full]
    type = FDP

    full = true

    solve_type = 'NEWTON'

    petsc_options_iname = '-pc_type -mat_fd_coloring_err -mat_fd_type'
    petsc_options_value = 'lu       1e-6                 ds'
  [../]
  [./FDP_jfnk]
    type = FDP

    full = true

    #Preconditioned JFNK (default)
    solve_type = 'PJFNK'

    petsc_options_iname = '-pc_type -mat_fd_coloring_err -mat_fd_type'
    petsc_options_value = 'lu       1e-6                 ds'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  [TimeIntegrator]
    type = GeneralizedAlpha
  []

  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-15
  nl_max_its = 10
  l_tol = 1e-5
  l_max_its = 500
  

  # Time Stepping information
  start_time = 0
  end_time = 100

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-4
    growth_factor = 1.25
    cutback_factor = 0.75
    optimal_iterations = 4
    linear_iteration_ratio = 100
  [../]
[]

[Outputs]
    perf_graph = true
    print_linear_residuals = false
    print_linear_converged_reason = true
    print_nonlinear_converged_reason = true
    print_nonlinear_residuals = true
    [Soln]
      type = Exodus
      execute_on = 'INITIAL TIMESTEP_END FINAL'
      time_step_interval = 10
    []
    [./console]
      type = Console
      output_file = true
    [../]
    # [debug] # This is a test, use the [Debug] block to enable this
    #   type = VariableResidualNormsDebugOutput
    # []
[]
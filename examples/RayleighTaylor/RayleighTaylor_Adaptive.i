[GlobalParams]
  L0 = 1e-0
  Ca = 0.02
  Mobility = 1e-7
  MonotonicPressure = true 
  rho2 = 3.0
  viscosity2 = 0.00135
  rho1 = 1.0
  viscosity1 = 0.00045
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    xmax = 1
    ymax = 4
    nx = 32
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
    function = '-tanh(sqrt(2.0)/2.0 * ((y - (2 - 0.05 * cos(2*pi*x))) / (0.02)))'
  [../]
[]


[BCs]
  [./Periodic]
    [./all]
      variable = 'mu phi u p'
      auto_direction = 'x'
    [../]
  [../]
  [./no_slip]
    type = ADVectorFunctionDirichletBC
    variable = u
    boundary = 'bottom top'
  [../]
[]

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

    solve_type = 'NEWTON'

    petsc_options_iname = '-ksp_type -pc_type -pc_factor_mat_solver_type'
    petsc_options_value = 'preonly   lu      mumps'
  []
  [./SMP]
    type = SMP
    full = true

    solve_type = 'NEWTON'

    petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -sub_pc_factor_levels -pc_asm_overlap -ksp_initial_guess_nonzero -ksp_knoll -ksp_converged_use_initial_residual_norm -ksp_gmres_cgs_refinement_type'
    petsc_options_value = 'asm      50                 preonly       ilu          2                     2               true                        true      true                                     refine_always '

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
  [TimeIntegrator]
    type = GeneralizedAlpha
  []

  residual_and_jacobian_together = true

  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-15
  nl_max_its = 10
  l_tol = 1e-5
  l_max_its = 300
  

  # Time Stepping information
  start_time = 0
  end_time = 100

  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-4
    growth_factor = 1.25
    cutback_factor = 0.75
    optimal_iterations = 4
    linear_iteration_ratio = 300
  [../]
[]

[Adaptivity]
  max_h_level = 2
  interval = 1
  #cycles_per_step = 2
  marker = combo
  initial_marker = combo
  initial_steps = 2
 [./Markers]
    [./combo]
      type = ComboMarker
      markers = 'mark_phi_grad value_marker'
    [../]
    [./value_marker]
      type = ValueRangeMarker
      lower_bound = -0.5
      upper_bound = 0.5
      buffer_size = 0.25
      variable = phi
    [../]
    [./mark_phi_grad]
      type = ErrorFractionMarker
      coarsen = 0.2
      refine = 0.6
      indicator = ind_phi_grad
    [../]
  [../]
  [./Indicators]
     [./ind_phi_grad]
       type = GradientJumpIndicator
       variable = phi
    [../]
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
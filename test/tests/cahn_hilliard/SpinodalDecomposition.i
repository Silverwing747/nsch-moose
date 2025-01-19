[GlobalParams]
  L0 = 1e-3
  Ca = 0.025
[]


[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
    elem_type = QUAD
  []
[]

[Variables]
    [./phi]
    [../]
    [./mu]
    [../]
[]

[Kernels]
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
      type = RandomIC
      variable = phi
      min = -0.1
      max =  0.1
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'mu phi'
      auto_direction = 'x y'
    [../]
  [../]
[]
  
[Materials]
  [./ADMaterial_NSCH]
    type = ADMaterial_NSCH
    phase_field = phi
    chemical_potential = mu
    output_properties = 'rho'
    outputs = Soln
  [../]
[]

[Preconditioning]
  [./SMP]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  scheme = NEWMARK-BETA

  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      100                preonly       lu           2'


  nl_rel_tol = 2.5e-4
  nl_abs_tol = 1e-15
  nl_max_its = 10
  l_tol = 1e-5
  l_max_its = 500

  # Time Stepping information
  start_time = 0
  end_time = 100
  [./TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-3
    growth_factor = 1.25
    cutback_factor = 0.75
    optimal_iterations = 4
    linear_iteration_ratio = 75
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
    []
    # [debug] # This is a test, use the [Debug] block to enable this
    #   type = VariableResidualNormsDebugOutput
    # []
[]
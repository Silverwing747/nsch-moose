[GlobalParams]
  L0 = 1e-0
  gravity = 0.0
  Inviscid = true
[]


[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
    elem_type = QUAD9
  []
[]
[Variables]
    [./u]
      family = LAGRANGE_VEC
      order = FIRST
    [../]
    [./p]
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
[]

[BCs]
  [./Periodic]
    [./all]
      variable = 'p u'
      auto_direction = 'x y'
    [../]
  [../]
[]
  
[Materials]
  [./ADMaterial_NSCH]
    type = ADMaterial_NSCH
    velocity = u
    pressure = p
    output_properties = 'rho'
    outputs = Soln
  [../]
[]

[ICs]
  [./pressure_IC]
    type = FunctionIC
    variable = p
    function = '0.25 * (cos(4*pi*x) + cos(4*pi*y))'
  [../]
  [./velocity_IC]
    type = VectorFunctionIC
    variable = u
    function_x = 'sin(2*pi*x) * cos(2*pi*y)'
    function_y = '-cos(2*pi*x) * sin(2*pi*y)'
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
  residual_and_jacobian_together = 1

  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      100                preonly       lu           2'


  nl_rel_tol = 2.5e-4
  nl_abs_tol = 1e-15
  nl_max_its = 10
  l_tol = 1e-5
  l_max_its = 500

  # Time Stepping information
  start_time = 0
  end_time = 1
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
[]
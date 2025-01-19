[GlobalParams]
  L0 = 1e-0
  gravity = 0.0
  sigma = 1.0
  rho1 = 1.0
  rho2 = 1.0
  viscosity1 = 0.00239808153
  viscosity2 = 0.00239808153
[]

[Mesh]
  [gen]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 128
    ny = 128
    elem_type = QUAD
  []
  [./corner_node]
    type = ExtraNodesetGenerator
    new_boundary = 'pinned_node'
    nodes = '0'
    input = gen
  [../]
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
  [./lid]
    type = VectorFunctionDirichletBC
    variable = u
    boundary = 'top'
    function_x = '1'
  [../]
  [./no_slip]
    type = VectorFunctionDirichletBC
    variable = u
    boundary = 'bottom right left'
  [../]
  [./pressure_pin]
    type = DirichletBC
    variable = p
    boundary = 'pinned_node'
    value = 0
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

[Preconditioning]
  [./SMP]
    type = SMP
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  scheme = bdf2

  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      100                preonly       lu           2'


  nl_rel_tol = 1e-4
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
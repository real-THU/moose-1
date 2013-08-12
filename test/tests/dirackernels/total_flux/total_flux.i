[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  # DiracKernels don't seem to work quite right in parallel with
  # ParallelMesh enabled (segfaults).  See #2125 for more details.
  distribution = serial
[]

[Variables]
  [./u]
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Postprocessors]
  [./dirac_reporter]
    type = Reporter
    sum = true
  [../]
[]

[Executioner]
  type = Steady
  petsc_options = '-snes_mf_operator -ksp_monitor'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Output]
  output_initial = true
  exodus = true
  perf_log = true
[]

[DiracKernels]
  [./source]
    reporter = dirac_reporter
    point = '0.2 0.2 0'
    value = 2
    variable = u
    type = ReportingConstantSource
  [../]
[]



project(
  'vulkan-tutorial', 
  'cpp',

  default_options : [ 
    'default_library=static',
    'cpp_std=c++23',
    'prefer_static=true'
  ]
)

cc = meson.get_compiler('cpp')


add_project_arguments('-O2', language : 'cpp')

if get_option('create_debug_symbols')
  add_project_arguments('-g', language : 'cpp')
endif

project_sources = []
project_header_files = []
project_libraries = []

inc = [ include_directories('inc') ]

subdir('inc')
subdir('src')

vulkan_dep = dependency('vulkan')
glfw3_dep = dependency('glfw3')

binary_dependencies = [
  vulkan_dep,
  glfw3_dep,
]


project_bin = executable(
  'tutorial',
  main_src,
  include_directories : inc,
  dependencies : binary_dependencies,
  link_with : project_libraries,
)



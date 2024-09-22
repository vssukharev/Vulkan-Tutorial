
main_src = files('main.cpp')

general_src = files('meta.cpp')
general_lib = static_library(
  'general',
  general_src,
  include_directories : inc,
)

project_libraries += general_lib
project_sources += general_src

subdir('helpers')
subdir('vulkan')
subdir('shaders')


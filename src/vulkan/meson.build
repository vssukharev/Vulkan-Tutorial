
graphics_source = files(
  'command-buffers.cpp',
  'command-pool.cpp',
  'drawer.cpp',
  'extensions-loader.cpp',
  'framebuffers.cpp',
  'general.cpp',
  'graphics-pipeline.cpp',
  'image-views.cpp',
  'instance.cpp',
  'logical-devices.cpp',
  'physical-devices.cpp',
  'queues.cpp',
  'render-pass.cpp',
  'swap-chain.cpp',
  'sync.cpp',
  'window.cpp',
  'vertex-processing.cpp',
  'vertex-buffer.cpp',
)

subdir('debug')

project_sources += graphics_source

graphics_lib = static_library(
  'graphics',
  graphics_source,
  include_directories : inc
)

project_libraries += graphics_lib



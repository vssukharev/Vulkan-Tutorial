


message('Enter shaders')
# 0 - source
# 1 - result
shaders = [ 
  { 'src' : 'shader.vert', 'out' : 'vert.spv' }, 
  { 'src' : 'shader.frag', 'out' : 'frag.spv' },
]

shaders_compile_cmd = [
  find_program('glslc'), 
  '@INPUT@', 
  '-o', 
  '@OUTPUT@'
]

foreach shader : shaders 
  custom_target(
    input: shader['src'],
    output: shader['out'],
    command: shaders_compile_cmd,
    install: true,
    install_dir: '.',
  )
endforeach


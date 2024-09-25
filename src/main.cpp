
#include <cstdlib>

#include <hello-triangle.hpp>
#include <except.hpp>
#include <helpers/mini-vec.hpp>


/// Initializes application
void App::Init(Data& data)
{
  Init(data.meta);
  Init(data.vulkan, data.meta);
}


/// Loops application
void App::MainLoop(Data& data) 
{
  while ( !glfwWindowShouldClose(data.vulkan.window) ) 
  {
    glfwPollEvents();
    DrawFrame(data);
  }

  vkDeviceWaitIdle(data.vulkan.device);
}


/// Collects garbafe left after application
void App::Cleanup(Data& data)
{
  Cleanup(data.vulkan);
}


/// Starting point
int main() {
  // App::Data data {};
  //
  // Init(data); 
  // MainLoop(data);
  // Cleanup(data);
  
  App::mini_vec<int> vec {1, 2, 3};

  vec[0] = 2;
  vec[2] = 4;
  vec.expand(1);
  vec[3] = 500;
  
  for (int elem : vec) {
    std::cout << elem << '\n';
  }
  std::cout << "---------\n";

  App::mini_vec<int> vec2 = vec;
  for (int elem : vec2) {
    std::cout << elem << '\n';
  }

  std::cout << "---------\n";
  App::mini_vec<int> vec3 {3, 2, 3, 4};
  for (int elem : vec3) {
    std::cout << elem << '\n';
  }
  std::cout << "---------\n";

  vec2 = vec3;
  for (int elem : vec2) {
    std::cout << elem << '\n';
  }

  return EXIT_SUCCESS;
}

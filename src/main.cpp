
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
  App::Data data {};

  Init(data); 
  // MainLoop(data);
  Cleanup(data);

  return EXIT_SUCCESS;
}

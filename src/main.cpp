

#include <hello-triangle.hpp>
#include <except.hpp>
#include <helpers/mini-vec.hpp>


/// Initializes application
void App::Init(Data& data)
{
  Init(data.meta);
  Init(data.vulkan, data.vulkan.dbg, data.meta);
}


/// Loops application
void App::MainLoop(Data& data) 
{
  while ( !glfwWindowShouldClose(data.vulkan.swap_chain.window) ) 
  {
    glfwPollEvents();
    DrawFrame(data);
  }

  vkDeviceWaitIdle(data.vulkan.swap_chain.device);
}


/// Collects garbafe left after application
void App::Cleanup(Data& data)
{
  Cleanup(data.vulkan, data.vulkan.dbg);
}


/// Starting point
int main() {
  App::Data data {};

  Init(data); 
  MainLoop(data);
  Cleanup(data);

  return 0;
}

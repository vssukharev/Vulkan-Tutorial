
#include "hello-triangle.hpp"
#include <GLFW/glfw3.h>
#include <hello-triangle.hpp>
#include <except.hpp>
#include <iostream>

#include <debug.hpp>
#include <vulkan/vulkan_core.h>


///
void App::InitGLFW()
{
  // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  glfwInit();
}

///
void App::CreateWindow(Window& window)
{
  // Prohibit GLFW creating OpenGL instance
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
  // Disable window resizing (for now) due to complexity of its handling
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
  window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

  Dbg::PrintFunctionInfo(__FUNCTION__, "GLFW window created");
}

///
void App::CreateSurface(VkSurfaceKHR& surface, Window window, VkInstance instance)
{
  if ( glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS )
    throw Except::Window_Surface_Creation_Failure{__PRETTY_FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Window surface created");
}


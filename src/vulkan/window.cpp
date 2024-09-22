
#include "hello-triangle.hpp"
#include <GLFW/glfw3.h>
#include <implementation.hpp>
#include <except.hpp>
#include <iostream>

#include <debug.hpp>


///
void App::InitGLFW()
{
  // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
  glfwInit();
}

///
void App::CreateWindow(Vulkan& vk)
{
  // Prohibit GLFW creating OpenGL instance
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
  // Disable window resizing (for now) due to complexity of its handling
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
  vk.window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);

  Dbg::PrintFunctionInfo(__FUNCTION__, "GLFW window created");
}

///
void App::CreateSurface(Vulkan& vk)
{
  if ( glfwCreateWindowSurface(vk.instance, vk.window, nullptr, &vk.surface) != VK_SUCCESS )
    throw Except::Window_Surface_Creation_Failure{__PRETTY_FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Window surface created");
}


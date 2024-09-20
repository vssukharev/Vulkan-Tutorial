
#include "hello-triangle.hpp"
#include <implementation.hpp>
#include <except.hpp>
#include <iostream>

#include <debug.hpp>

///
void App::CreateWindow(Vulkan& vk)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Prohibit GLFW creating OpenGL instance
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Disable window resizing (for now) due to complexity of its handling
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


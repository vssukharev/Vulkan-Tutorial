
#include "decl.hpp"
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
void App::CreateWindow(
    Window&         rWindow,
    SWAPCHAIN_STATE_BITS_T& pSwapChainState)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 
  rWindow = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
  
  glfwSetWindowUserPointer(rWindow, &pSwapChainState);
  glfwSetFramebufferSizeCallback(rWindow, FramebufferResizeCallback);
  
  Dbg::PrintFunctionInfo(__FUNCTION__, "GLFW window created");
}

///
void App::CreateSurface(VkSurfaceKHR& rSurface, Window window, VkInstance instance)
{
  if ( glfwCreateWindowSurface(instance, window, nullptr, &rSurface) != VK_SUCCESS )
    throw Except::Window_Surface_Creation_Failure{__PRETTY_FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Window surface created");
}



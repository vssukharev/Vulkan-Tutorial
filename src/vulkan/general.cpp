
#include "hello-triangle.hpp"
#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>


/// Initializes Vulkan instance and GLFW surface
void App::Init(Vulkan& vk, Meta& meta)
{
  InitGLFW();
  CreateWindow(vk);
  CreateInstance(vk);
  Dbg::CreateDebugMessenger(vk);
  CreateSurface(vk);
  PickPhysicalDevice(vk);
  CreateLogicalDevice(vk);
  CreateSwapChain(vk);
  CreateImageViews(vk);
  CreateRenderPass(vk);
  CreateGraphicsPipeline(vk, meta);
  CreateFramebuffers(vk);
  CreateCommandPool(vk);
  CreateCommandBuffers(vk);
  CreateSyncObjects(vk);
}


///
void App::Cleanup(Vulkan& vk) noexcept
{
  for (auto image_available_semaphore : vk.sync.image_available_semaphores)
    vkDestroySemaphore(vk.device, image_available_semaphore, nullptr);
  
  for (auto render_finished_semaphore : vk.sync.render_finished_semaphores)
    vkDestroySemaphore(vk.device, render_finished_semaphore, nullptr);
  
  for (auto in_flight_fence : vk.sync.in_flight_fences)
    vkDestroyFence(vk.device, in_flight_fence, nullptr);
  
  vkDestroyCommandPool(vk.device, vk.command_pool, nullptr);
  
  for (auto framebuffer : vk.swap_chain.framebuffers)
    vkDestroyFramebuffer(vk.device, framebuffer, nullptr);
  
  vkDestroyPipeline(vk.device, vk.graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(vk.device, vk.pipeline_layout, nullptr);
  vkDestroyRenderPass(vk.device, vk.render_pass, nullptr);
  
  for (auto image_view : vk.swap_chain.image_views)
    vkDestroyImageView(vk.device, image_view, nullptr);
  
  vkDestroySwapchainKHR(vk.device, vk.swap_chain.handle, nullptr);
  vkDestroyDevice(vk.device, nullptr);
  Dbg::DestroyDebugMessenger(vk);
  vkDestroySurfaceKHR(vk.instance, vk.surface, nullptr);
  vkDestroyInstance(vk.instance, nullptr);
  glfwDestroyWindow(vk.window);
  glfwTerminate();
}




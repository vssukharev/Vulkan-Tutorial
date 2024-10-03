
#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <hello-triangle.hpp>
#include <except.hpp>
#include <debug.hpp>


/// Initializes Vulkan instance and GLFW surface
void App::Init(Vulkan& rVk, VulkanDebug& rVkDbg, const Meta& meta)
{
  InitGLFW();
  CreateWindow(
      rVk.swap_chain.window,
      rVk.swap_chain.state);
  CreateInstance(
      rVk.instance);
  Dbg::CreateDebugMessenger(
      rVkDbg.messenger, 
      rVk.instance);
  CreateSurface(
      rVk.swap_chain.surface, 
      rVk.swap_chain.window, 
      rVk.instance);
  PickPhysicalDevice(
      rVk.swap_chain.physical_device, 
      rVk.instance, 
      rVk.swap_chain.surface);
  SetQueueFamilies(
      rVk.swap_chain.queue_families, 
      rVk.swap_chain.physical_device, 
      rVk.swap_chain.surface);
  CreateLogicalDevice(
      rVk.swap_chain.device, 
      rVk.swap_chain.queue_families, 
      rVk.swap_chain.physical_device, 
      rVk.swap_chain.surface);
  SetQueues(
      rVk.queues, 
      rVk.swap_chain.queue_families, 
      rVk.swap_chain.device);
  CreateSwapChainComponents(
      rVk.swap_chain);
  CreateGraphicsPipeline(
      rVk.graphics_pipeline, 
      rVk.pipeline_layout, 
      rVk.swap_chain.device, 
      rVk.swap_chain.render_pass, 
      meta.binary_dir);
  CreateCommandPools(
      rVk.command_pools, 
      rVk.swap_chain.queue_families, 
      rVk.swap_chain.device);
  SetVertices(
      rVk.vertices);
  CreateVertexBuffer(
      rVk.vertex_buffer, 
      rVk.vertex_buffer_mem, 
      rVk.vertices,
      rVk.swap_chain.queue_families,
      rVk.swap_chain.device, 
      rVk.swap_chain.physical_device);
  CreateCommandBuffers(
      rVk.command_buffers, 
      rVk.swap_chain.device, 
      rVk.command_pools);
  CreateSyncObjects(
      rVk.sync, 
      rVk.swap_chain.device);
}


///
void App::Cleanup(Vulkan& rVk, VulkanDebug& rVkDbg) noexcept
{
  CleanupSwapChain(
      rVk.swap_chain.framebuffers, 
      rVk.swap_chain.image_views, 
      rVk.swap_chain.handle, 
      rVk.swap_chain.device);

  vkDestroyBuffer(rVk.swap_chain.device, rVk.vertex_buffer, nullptr);
  vkFreeMemory(rVk.swap_chain.device, rVk.vertex_buffer_mem, nullptr);

  vkDestroyCommandPool(rVk.swap_chain.device, rVk.command_pools.graphics, nullptr);
  vkDestroyCommandPool(rVk.swap_chain.device, rVk.command_pools.transfer, nullptr);
  
  vkDestroyPipeline(rVk.swap_chain.device, rVk.graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(rVk.swap_chain.device, rVk.pipeline_layout, nullptr);
  vkDestroyRenderPass(rVk.swap_chain.device, rVk.swap_chain.render_pass, nullptr);
  
  for (auto image_available_semaphore : rVk.sync.image_available_semaphores)
    vkDestroySemaphore(rVk.swap_chain.device, image_available_semaphore, nullptr);
  for (auto render_finished_semaphore : rVk.sync.render_finished_semaphores)
    vkDestroySemaphore(rVk.swap_chain.device, render_finished_semaphore, nullptr);
  for (auto in_flight_fence : rVk.sync.in_flight_fences)
    vkDestroyFence(rVk.swap_chain.device, in_flight_fence, nullptr);
  
  vkDestroyDevice(rVk.swap_chain.device, nullptr);
  Dbg::DestroyDebugMessenger(rVkDbg.messenger, rVk.instance);
  vkDestroySurfaceKHR(rVk.instance, rVk.swap_chain.surface, nullptr);
  vkDestroyInstance(rVk.instance, nullptr);
  glfwDestroyWindow(rVk.swap_chain.window);
  glfwTerminate();
}




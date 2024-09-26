
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
  CreateWindow(rVk.window);
  CreateInstance(rVk.instance);
  Dbg::CreateDebugMessenger(rVkDbg.messenger, rVk.instance);
  CreateSurface(rVk.surface, rVk.window, rVk.instance);
  PickPhysicalDevice(rVk.physical_device, rVk.instance, rVk.surface);
  SetQueueFamilies(rVk.queue_families, rVk.physical_device, rVk.surface);
  CreateLogicalDevice(rVk.device, rVk.queue_families, rVk.physical_device, rVk.surface);
  SetQueues(rVk.queues, rVk.queue_families, rVk.device);
  CreateSwapChain(rVk.swap_chain, rVk.images, rVk.image_format, rVk.extent, rVk.device, rVk.physical_device, rVk.surface, rVk.window, rVk.queue_families);
  CreateImageViews(rVk.image_views, rVk.images, rVk.image_format, rVk.device);
  CreateRenderPass(rVk.render_pass, rVk.image_format, rVk.device);
  CreateGraphicsPipeline(rVk.graphics_pipeline, rVk.pipeline_layout, rVk.device, rVk.render_pass, meta.binary_dir);
  CreateFramebuffers(rVk.framebuffers, rVk.image_views, rVk.device, rVk.render_pass, rVk.extent);
  CreateCommandPool(rVk.command_pool, rVk.queue_families, rVk.device);
  CreateCommandBuffers(rVk.command_buffers, rVk.device, rVk.command_pool);
  CreateSyncObjects(rVk.sync, rVk.device);
}


///
void App::Cleanup(Vulkan& rVk, VulkanDebug& rVkDbg) noexcept
{
  for (auto image_available_semaphore : rVk.sync.image_available_semaphores)
    vkDestroySemaphore(rVk.device, image_available_semaphore, nullptr);
  
  for (auto render_finished_semaphore : rVk.sync.render_finished_semaphores)
    vkDestroySemaphore(rVk.device, render_finished_semaphore, nullptr);
  
  for (auto in_flight_fence : rVk.sync.in_flight_fences)
    vkDestroyFence(rVk.device, in_flight_fence, nullptr);
  
  vkDestroyCommandPool(rVk.device, rVk.command_pool, nullptr);
  
  for (auto framebuffer : rVk.framebuffers)
    vkDestroyFramebuffer(rVk.device, framebuffer, nullptr);
  
  vkDestroyPipeline(rVk.device, rVk.graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(rVk.device, rVk.pipeline_layout, nullptr);
  vkDestroyRenderPass(rVk.device, rVk.render_pass, nullptr);
  
  for (auto image_view : rVk.image_views)
    vkDestroyImageView(rVk.device, image_view, nullptr);
  
  vkDestroySwapchainKHR(rVk.device, rVk.swap_chain, nullptr);
  vkDestroyDevice(rVk.device, nullptr);
  Dbg::DestroyDebugMessenger(rVkDbg.messenger, rVk.instance);
  vkDestroySurfaceKHR(rVk.instance, rVk.surface, nullptr);
  vkDestroyInstance(rVk.instance, nullptr);
  glfwDestroyWindow(rVk.window);
  glfwTerminate();
}




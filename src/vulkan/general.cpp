
#include <cstdint>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <hello-triangle.hpp>
#include <except.hpp>
#include <debug.hpp>


/// Initializes Vulkan instance and GLFW surface
void App::Init(Vulkan& vk, Meta& meta)
{
  InitGLFW();
  CreateWindow(vk.window);
  CreateInstance(vk.instance);
  Dbg::CreateDebugMessenger(vk);
  CreateSurface(vk.surface, vk.window, vk.instance);
  PickPhysicalDevice(vk.physical_device, vk.instance, vk.surface);
  SetQueueFamilies(vk.queue_families, vk.physical_device, vk.surface);
  CreateLogicalDevice(vk.device, vk.queue_families, vk.physical_device, vk.surface);
  SetQueues(vk.queues, vk.queue_families, vk.device);
  CreateSwapChain(vk.swap_chain, vk.images, vk.image_format, vk.extent, vk.device, vk.physical_device, vk.surface, vk.window, vk.queue_families);
  CreateImageViews(vk.image_views, vk.images, vk.image_format, vk.device);
  CreateRenderPass(vk.render_pass, vk.image_format, vk.device);
  CreateGraphicsPipeline(vk.graphics_pipeline, vk.pipeline_layout, vk.device, vk.render_pass, meta.binary_dir);
  CreateFramebuffers(vk.framebuffers, vk.image_views, vk.device, vk.render_pass, vk.extent);
  CreateCommandPool(vk.command_pool, vk.queue_families, vk.device);
  CreateCommandBuffers(vk.command_buffers, vk.device, vk.command_pool);
  CreateSyncObjects(vk.sync, vk.device);
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
  
  for (auto framebuffer : vk.framebuffers)
    vkDestroyFramebuffer(vk.device, framebuffer, nullptr);
  
  vkDestroyPipeline(vk.device, vk.graphics_pipeline, nullptr);
  vkDestroyPipelineLayout(vk.device, vk.pipeline_layout, nullptr);
  vkDestroyRenderPass(vk.device, vk.render_pass, nullptr);
  
  for (auto image_view : vk.image_views)
    vkDestroyImageView(vk.device, image_view, nullptr);
  
  vkDestroySwapchainKHR(vk.device, vk.swap_chain, nullptr);
  vkDestroyDevice(vk.device, nullptr);
  Dbg::DestroyDebugMessenger(vk);
  vkDestroySurfaceKHR(vk.instance, vk.surface, nullptr);
  vkDestroyInstance(vk.instance, nullptr);
  glfwDestroyWindow(vk.window);
  glfwTerminate();
}





#include "decl.hpp"
#include <cstdint>

#include <except.hpp>
#include <debug.hpp>
#include <helpers/str.hpp>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>

void App::DrawFrame(Data& data)
{
  DrawFrame(
      data.vulkan.current_frame, 
      data.vulkan.command_buffers, 
      data.vulkan.swap_chain,
      data.vulkan.sync, 
      data.vulkan.queues, 
      data.vulkan.graphics_pipeline);
}


void App::DrawFrame(
    uint32_t& rLastFrame,
    CommandBuffers& rCommandBuffers,
    SwapChainComponents& rSwapChain,
    const SyncObjects& sync,
    const Queues& queues,
    VkPipeline graphicsPipeline)
{
  vkWaitForFences(
      rSwapChain.device, 
      1, &sync.in_flight_fences[rLastFrame], 
      VK_TRUE, 
      UINT64_MAX);

  VkResult result;
  uint32_t image_index;
  
  result = vkAcquireNextImageKHR(
      rSwapChain.device, 
      rSwapChain.handle, 
      UINT64_MAX, 
      sync.image_available_semaphores[rLastFrame], 
      VK_NULL_HANDLE, 
      &image_index);

  if (result == VK_ERROR_OUT_OF_DATE_KHR  || 
      rSwapChain.state & SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED) 
  {
    Dbg::PrintFunctionInfo(__FUNCTION__, "Recreating swap chain");
    rSwapChain.state &= ~SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED;
    RecreateSwapChainComponents(rSwapChain);
    return;
  } else if (result != VK_SUCCESS && 
             result != VK_SUBOPTIMAL_KHR)
    throw Except::Image_Acquiring_Failure{__FUNCTION__};
  

  vkResetFences(
      rSwapChain.device, 
      1, &sync.in_flight_fences[rLastFrame]);

  vkResetCommandBuffer(rCommandBuffers[rLastFrame], 0);
  RecordCommandBuffer(
      rCommandBuffers[rLastFrame], 
      rSwapChain.framebuffers[image_index], 
      rSwapChain.extent, 
      rSwapChain.render_pass, 
      graphicsPipeline);
 

  VkPipelineStageFlags wait_stages[] = {
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
  };

  VkSubmitInfo queue_submit_info {};
  queue_submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  queue_submit_info.waitSemaphoreCount = 1;
  queue_submit_info.pWaitSemaphores = &sync.image_available_semaphores[rLastFrame];
  queue_submit_info.signalSemaphoreCount = 1;
  queue_submit_info.pSignalSemaphores = &sync.render_finished_semaphores[rLastFrame];
  queue_submit_info.pWaitDstStageMask = wait_stages;
  queue_submit_info.commandBufferCount = 1;
  queue_submit_info.pCommandBuffers = &rCommandBuffers[rLastFrame];


  result = vkQueueSubmit(
      queues.graphics_queue, 
      1, 
      &queue_submit_info, 
      sync.in_flight_fences[rLastFrame]);
  
  if (result != VK_SUCCESS)
    throw Except::Queue_Submittion_Failure{__FUNCTION__};
  

  VkPresentInfoKHR present_info {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &sync.render_finished_semaphores[rLastFrame];
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &rSwapChain.handle;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;

  result = vkQueuePresentKHR(
      queues.presentation_queue, 
      &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || 
      result == VK_SUBOPTIMAL_KHR        ||
      rSwapChain.state & SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED)
  {
    // FIX: doesn't show third argument in cout
    Dbg::PrintFunctionInfo(__FUNCTION__, "Recreating swap chain");
    rSwapChain.state &= ~SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED;
    RecreateSwapChainComponents(rSwapChain);
  } else if (result != VK_SUCCESS)
    throw Except::Image_Presentation_Failure{__FUNCTION__};

  rLastFrame = (rLastFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


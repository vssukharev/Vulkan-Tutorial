
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
      data.vulkan.sync, 
      data.vulkan.queues, 
      data.vulkan.framebuffers, 
      data.vulkan.device, 
      data.vulkan.swap_chain, 
      data.vulkan.extent, 
      data.vulkan.render_pass, 
      data.vulkan.graphics_pipeline);
}


void App::DrawFrame(
    uint32_t& rLastFrame,
    CommandBuffers& rCommandBuffers,
    const SyncObjects& sync,
    const Queues& queues,
    const Framebuffers& framebuffers,
    VkDevice logicalDevice,
    VkSwapchainKHR swapChain,
    VkExtent2D imageExtent,
    VkRenderPass renderPass,
    VkPipeline graphicsPipeline)
{
  // We pass an array of fences. VK_TRUE shows that we are waiting for all fences but if we pass only one fence,, it doesn't matter. Set last parameter - timeout - to maximum, disabling it
  
  vkWaitForFences(logicalDevice, 1, &sync.in_flight_fences[rLastFrame], VK_TRUE, UINT64_MAX);
  vkResetFences(logicalDevice, 1, &sync.in_flight_fences[rLastFrame]);

  uint32_t image_index;
  // Acquire image from GPU and set 'image_available_semaphore' as submit one
  vkAcquireNextImageKHR(logicalDevice, swapChain, UINT64_MAX, sync.image_available_semaphores[rLastFrame], VK_NULL_HANDLE, &image_index);

  // Dbg::PrintFunctionInfo(__FUNCTION__, "vkAcquireNextImageKHR result - ", std::boolalpha, res); 
  

  vkResetCommandBuffer(rCommandBuffers[rLastFrame], 0);
  RecordCommandBuffer(rCommandBuffers[rLastFrame], framebuffers[rLastFrame], imageExtent, renderPass, graphicsPipeline);
  
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {
    sync.image_available_semaphores[rLastFrame]
  };

  VkSemaphore signal_semaphores[] = {
    sync.render_finished_semaphores[rLastFrame]
  };

  VkPipelineStageFlags wait_stages[] = {
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
  };

  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = wait_semaphores;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = signal_semaphores;
  submit_info.pWaitDstStageMask = wait_stages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &rCommandBuffers[rLastFrame];

  // in_flight_fence will be signaled when the command buffer finish execution
  if ( vkQueueSubmit(queues.graphics_queue, 1, &submit_info, sync.in_flight_fences[rLastFrame]) != VK_SUCCESS )
    throw Except::Queue_Submittion_Failure{Impl::ConcatToStr(__FUNCTION__, " - failed to submit draw command buffer").c_str()};
  
  // Dbg::PrintFunctionInfo(__FUNCTION__, "Submitted draw command buffer");

  VkPresentInfoKHR present_info {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &swapChain;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;

  // TODO: check for errors
  vkQueuePresentKHR(queues.presentation_queue, &present_info);


  // Dbg::PrintFunctionInfo(__FUNCTION__, "vkQueuePresentKHR result - ", std::boolalpha, res); 
  rLastFrame = (rLastFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}


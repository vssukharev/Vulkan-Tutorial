
#include "config.hpp"
#include "hello-triangle.hpp"
#include <cstdint>
#include <ios>
#include <limits>
#include <vulkan/vulkan_core.h>


#include <except.hpp>
#include <debug.hpp>
#include <implementation.hpp>
#include <helpers/str.hpp>

void App::DrawFrame(Vulkan& vk)
{
  uint32_t current_frame = vk.frame.current;
  // We pass an array of fences. VK_TRUE shows that we are waiting for all fences but if we pass only one fence,, it doesn't matter. Set last parameter - timeout - to maximum, disabling it
  
  vkWaitForFences(vk.device, 1, &vk.sync.in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);
  vkResetFences(vk.device, 1, &vk.sync.in_flight_fences[current_frame]);

  uint32_t image_index;
  // Acquire image from GPU and set 'image_available_semaphore' as submit one
  vkAcquireNextImageKHR(vk.device, vk.swap_chain.handle, UINT64_MAX, vk.sync.image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

  // Dbg::PrintFunctionInfo(__FUNCTION__, "vkAcquireNextImageKHR result - ", std::boolalpha, res); 
  

  vkResetCommandBuffer(vk.command_buffers[current_frame], 0);
  RecordCommandBuffer(vk, current_frame, image_index);
  
  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[] = {
    vk.sync.image_available_semaphores[current_frame]
  };

  VkSemaphore signal_semaphores[] = {
    vk.sync.render_finished_semaphores[current_frame]
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
  submit_info.pCommandBuffers = &vk.command_buffers[current_frame];

  // in_flight_fence will be signaled when the command buffer finish execution
  if ( vkQueueSubmit(vk.queues.graphics_queue, 1, &submit_info, vk.sync.in_flight_fences[current_frame]) != VK_SUCCESS )
    throw Except::Queue_Submittion_Failure{Impl::ConcatToStr(__FUNCTION__, " - failed to submit draw command buffer").c_str()};
  
  // Dbg::PrintFunctionInfo(__FUNCTION__, "Submitted draw command buffer");

  VkPresentInfoKHR present_info {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = signal_semaphores;
  VkSwapchainKHR swap_chains[] = {vk.swap_chain.handle};
  present_info.swapchainCount = 1;
  present_info.pSwapchains = swap_chains;
  present_info.pImageIndices = &image_index;
  present_info.pResults = nullptr;

  // TODO: check for errors
  vkQueuePresentKHR(vk.queues.presentation_queue, &present_info);

  // Dbg::PrintFunctionInfo(__FUNCTION__, "vkQueuePresentKHR result - ", std::boolalpha, res); 
  vk.frame.current = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}


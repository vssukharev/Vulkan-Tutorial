
#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>

void App::CreateSyncObjects(Vulkan& vk)
{
  VkSemaphoreCreateInfo semaphore_info {};
  semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_info {};
  fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  if (vkCreateSemaphore(vk.device, &semaphore_info, nullptr, &vk.sync.image_available_semaphore) != VK_SUCCESS ||
      vkCreateSemaphore(vk.device, &semaphore_info, nullptr, &vk.sync.render_finished_semaphore) != VK_SUCCESS ||
      vkCreateFence(vk.device, &fence_info, nullptr, &vk.sync.in_flight_fence) != VK_SUCCESS)
    throw Except::Semaphore_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created synchronization objects");
}

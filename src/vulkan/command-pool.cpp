
#include "hello-triangle.hpp"
#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>

void App::CreateCommandPool(Vulkan& vk)
{
  SetQueueFamilies(vk);

  VkCommandPoolCreateInfo poolinfo {};
  poolinfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  // flags can be:
  // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT - command buffers are rerecorded with new commands very often
  // VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT - allow command buffers to be rerecorded individually, without this flag they all havee to be reset together
  poolinfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolinfo.queueFamilyIndex = vk.queue_families.graphics_family;

  if (vkCreateCommandPool(vk.device, &poolinfo, nullptr, &vk.command_pool) != VK_SUCCESS)
    throw Except::Command_Pool_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created command pool");
}



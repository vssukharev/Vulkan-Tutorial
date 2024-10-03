
#include <vulkan/vulkan_core.h>

#include <except.hpp>
#include <debug.hpp>
#include <hello-triangle.hpp>

void App::CreateCommandPools(
  CommandPools&         rCommandPool,
  const QueueFamilies&  queueFamilies,
  VkDevice              logicalDevice)
{
  VkCommandPoolCreateInfo graphics_pool_info {};
  graphics_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  graphics_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  graphics_pool_info.queueFamilyIndex = queueFamilies.graphics;

  if (vkCreateCommandPool(logicalDevice, &graphics_pool_info, nullptr, &rCommandPool.graphics) != VK_SUCCESS)
    throw Except::Command_Pool_Creation_Failure{"Graphics"};
  
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created graphics command pool");


  VkCommandPoolCreateInfo transfer_pool_info {};
  transfer_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  transfer_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  transfer_pool_info.queueFamilyIndex = queueFamilies.transfer;

  if (vkCreateCommandPool(logicalDevice, &transfer_pool_info, nullptr, &rCommandPool.transfer) != VK_SUCCESS)
    throw Except::Command_Pool_Creation_Failure{"Transfer"};

  Dbg::PrintFunctionInfo(__FUNCTION__, "Created transfer command pool");
}



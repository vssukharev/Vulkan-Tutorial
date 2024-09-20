
#include "hello-triangle.hpp"
#include <cstdint>
#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>

#include <config.hpp>
#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>

/// Creates logical device and assigns it to vk.device
/// @throw Except::Instance_Failure
void App::CreateLogicalDevice(Vulkan& vk)
{
  SetQueueFamilies(vk);

  auto queue_create_infos = Impl::GetVulkanQueueCreateInfos(vk.queue_families);

  // Leave empty as we don't need any device features
  VkPhysicalDeviceFeatures device_features {};

  VkDeviceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.queueCreateInfoCount = queue_create_infos.size();
  create_info.pEnabledFeatures = &device_features;
  create_info.enabledExtensionCount = gl_device_extensions.size();
  create_info.ppEnabledExtensionNames = gl_device_extensions.data();
  create_info.enabledLayerCount = Dbg::gl_validation_layers.size();
  create_info.ppEnabledLayerNames = Dbg::gl_validation_layers.data();

  if ( vkCreateDevice(vk.physical_device, &create_info, nullptr, &vk.device) != VK_SUCCESS )
    throw Except::Logical_Device_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created logical device");
  
  vkGetDeviceQueue(vk.device, vk.queue_families.graphics_family, 0, &vk.queues.graphics_queue);
  vkGetDeviceQueue(vk.device, vk.queue_families.present_family, 0, &vk.queues.presentation_queue);
}


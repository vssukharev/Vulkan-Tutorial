
#include <cstdint>
#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>

#include <config.hpp>
#include <hello-triangle.hpp>
#include <except.hpp>
#include <debug.hpp>

/// Creates logical device and assigns it to vk.device
/// @throw Except::Instance_Failure
void App::CreateLogicalDevice(
    VkDevice& device, 
    QueueFamilies& qf,
    VkPhysicalDevice physical_device, 
    VkSurfaceKHR surface)
{
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos {};
  
  std::set<uint32_t> unique_queue_families { 
    qf.graphics_family, 
    qf.present_family 
  };

  float queue_priority = 1.0f;
  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

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

  if ( vkCreateDevice(physical_device, &create_info, nullptr, &device) != VK_SUCCESS )
    throw Except::Logical_Device_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created logical device");
}


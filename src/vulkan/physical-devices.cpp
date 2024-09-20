
#include "hello-triangle.hpp"
#include <cstdint>
#include <map>
#include <cstring>
#include <iostream>

#include <implementation.hpp>
#include <except.hpp>
#include <vulkan/vulkan_core.h>
#include <debug.hpp>

/// Picks physical device (GPU, CPU or other)
/// @throw Except::Device_Failure
void App::PickPhysicalDevice(Vulkan& vk)
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vk.instance, &device_count, nullptr);
  
  if (device_count == 0)
    throw Except::Unsupported_Physical_Devices{"zero devices found"};

  VkPhysicalDevice devices[device_count];
  vkEnumeratePhysicalDevices(vk.instance, &device_count, devices);

  // Choose multimap to set all candidate scores and choose the best one
  std::multimap<int, VkPhysicalDevice> candidates;

  // Create temporary queue families variable
  Vulkan_QueueFamilies tmp_qf {};
  Impl::Vulkan_SwapChainSupportDetails tmp_sc_details {};

  // Loop through devices
  for (const auto& device : devices) 
  {
    Impl::QueryQueueFamilies(tmp_qf, device, vk.surface);
    Impl::QuerySwapChainSupport(tmp_sc_details, device, vk.surface);

    int score = Impl::RatePhysicalDeviceSuitability(device, tmp_qf, tmp_sc_details);
    candidates.insert(std::make_pair(score, device));
  }

  if (candidates.rbegin()->first > 0)
    vk.physical_device = candidates.rbegin()->second;
  else
    throw Except::Unsupported_Physical_Devices{"device(s) found but none of them suits"};

  Dbg::PrintFunctionInfo(__FUNCTION__, "Best matched physical device score = ", candidates.rbegin()->first);
}


/// Device property priorities: DISCRETE_GPU > INTEGRATED_GPU > VIRTUAL_GPU > CPU > OTHER.
/// @return - 0 (if device property is invalid or some feature is not supported)
/// @return - score (otherwise)
int App::Impl::RatePhysicalDeviceSuitability(VkPhysicalDevice dev, Vulkan_QueueFamilies& qf, Vulkan_SwapChainSupportDetails& sc_details) noexcept
{
  VkPhysicalDeviceProperties dev_properties;
  VkPhysicalDeviceFeatures dev_features;
  vkGetPhysicalDeviceProperties(dev, &dev_properties);
  vkGetPhysicalDeviceFeatures(dev, &dev_features);
 
  int score = Impl::CheckQueueFamiliesSupport(qf) && 
              Impl::CheckPhysicalDeviceExtensionsSupport(dev) &&
              Impl::CheckSwapChainSupport(sc_details);

  // if ( !dev_features.geometryShader ) score = 0; 

  if (score != 0)
    switch (dev_properties.deviceType) {
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: score = 10000; break;
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score = 1000; break;
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: score = 100; break;
      case VK_PHYSICAL_DEVICE_TYPE_CPU: score = 10; break;
      case VK_PHYSICAL_DEVICE_TYPE_OTHER: score = 1; break;
      default: break;
    }

  return score;
}


///
bool App::Impl::CheckPhysicalDeviceExtensionsSupport(VkPhysicalDevice dev)
{
  uint32_t extension_count = 0;
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);
  VkExtensionProperties available_extensions[extension_count];
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions);

  std::vector<const char*> unavailable_extensions {};

  for (const char* required_extension : gl_device_extensions) {
    bool extension_found = false;

    for (const auto& available_extension : available_extensions)
      if ( strcmp(required_extension, available_extension.extensionName) == 0 ) {
        extension_found = true;
        break;
      }

    if (!extension_found) {
      unavailable_extensions.push_back(required_extension);
    }
  }

  return unavailable_extensions.size() == 0;
}






#include <cstdint>
#include <map>
#include <cstring>
#include <iostream>

#include <hello-triangle.hpp>
#include <except.hpp>
#include <vulkan/vulkan_core.h>
#include <debug.hpp>

/// Picks physical device (GPU, CPU or other)
/// @throw Except::Device_Failure
void App::PickPhysicalDevice(VkPhysicalDevice& physical_device, VkInstance instance, VkSurfaceKHR surface)
{
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
  
  if (device_count == 0)
    throw Except::Unsupported_Physical_Devices{"zero devices found"};

  std::vector<VkPhysicalDevice> available_devices(device_count);
  vkEnumeratePhysicalDevices(instance, &device_count, available_devices.data());

  // Choose multimap to set all candidate scores and choose the best one
  std::multimap<int, VkPhysicalDevice> candidates;

  // Create temporary queue families variable
  QueueFamilies tmp_qf {};
  SwapChainSupportDetails tmp_sc_details {};

  // Loop through devices
  for (const auto& available_device : available_devices) 
  {
    QueryQueueFamilies(tmp_qf, available_device, surface);
    QuerySwapChainSupport(tmp_sc_details, available_device, surface);

    int score = RatePhysicalDeviceSuitability(available_device, tmp_qf, tmp_sc_details);
    candidates.insert(std::make_pair(score, available_device));
  }

  if (candidates.rbegin()->first > 0)
    physical_device = candidates.rbegin()->second;
  else
    throw Except::Unsupported_Physical_Devices{"device(s) found but none of them suits"};

  Dbg::PrintFunctionInfo(__FUNCTION__, "Best matched physical device score = ", candidates.rbegin()->first);
}


/// Device property priorities: DISCRETE_GPU > INTEGRATED_GPU > VIRTUAL_GPU > CPU > OTHER.
/// @return - 0 (if device property is invalid or some feature is not supported)
/// @return - score (otherwise)
int App::RatePhysicalDeviceSuitability(VkPhysicalDevice dev, QueueFamilies& qf, SwapChainSupportDetails& sc_details) noexcept
{
  VkPhysicalDeviceProperties dev_properties;
  VkPhysicalDeviceFeatures dev_features;
  vkGetPhysicalDeviceProperties(dev, &dev_properties);
  vkGetPhysicalDeviceFeatures(dev, &dev_features);
 
  int score = CheckQueueFamiliesSupport(qf) && 
              CheckPhysicalDeviceExtensionsSupport(dev) &&
              CheckSwapChainSupport(sc_details);

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
bool App::CheckPhysicalDeviceExtensionsSupport(VkPhysicalDevice dev)
{
  uint32_t extension_count = 0;
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, available_extensions.data());

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


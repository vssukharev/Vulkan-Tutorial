
#pragma once

#include <cstdint>

#include <cwchar>
#include <hello-triangle.hpp>
#include <config.hpp>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace App::Impl {
  
  // ------ Vulkan ------
  // --- Helper structs
  struct Vulkan_SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };
  
  // --- Extensions
  std::vector<const char*> GetRequiredVulkanExtensions() noexcept;

  // --- Devices
  int RatePhysicalDeviceSuitability(VkPhysicalDevice, Vulkan_QueueFamilies&, Vulkan_SwapChainSupportDetails&) noexcept;
  bool CheckPhysicalDeviceExtensionsSupport(VkPhysicalDevice);


  // --- Queue Families
  void QueryQueueFamilies(Vulkan_QueueFamilies&, VkPhysicalDevice, VkSurfaceKHR);
  bool CheckQueueFamiliesSupport(const Vulkan_QueueFamilies&);
  std::vector<VkDeviceQueueCreateInfo> GetVulkanQueueCreateInfos(Vulkan_QueueFamilies&);

  // --- Swap chain
  void QuerySwapChainSupport(Vulkan_SwapChainSupportDetails&, VkPhysicalDevice, VkSurfaceKHR);
  bool CheckSwapChainSupport(const Vulkan_SwapChainSupportDetails&);
  
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>&);
  int RateSwapSurfaceFormat(VkSurfaceFormatKHR);
  
  VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>&);
  int RateSwapPresentMode(VkPresentModeKHR);
  
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR&, GLFWwindow*);

  void CreateImageViews(Vulkan_SwapChain&, VkDevice);

  // --- Shaders
  VkShaderModule CreateShaderModule(VkDevice dev, const std::vector<char>& code);
  // --------------------

}



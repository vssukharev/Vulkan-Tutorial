  
#include "hello-triangle.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

#include <implementation.hpp>
#include <except.hpp>
#include <vulkan/vulkan_core.h>
#include <debug.hpp>

/// Creates swap chain
void App::CreateSwapChain(Vulkan& vk)
{
  Impl::Vulkan_SwapChainSupportDetails swap_chain_support {};
  Impl::QuerySwapChainSupport(swap_chain_support, vk.physical_device, vk.surface);
  
  VkSurfaceFormatKHR surface_format = Impl::ChooseSwapSurfaceFormat(swap_chain_support.formats);
  VkPresentModeKHR present_mode = Impl::ChooseSwapPresentMode(swap_chain_support.present_modes);
  VkExtent2D extent = Impl::ChooseSwapExtent(swap_chain_support.capabilities, vk.window);

  // We add 1 to minimal image count to avoid waiting for driver to render one more image
  uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
  if (swap_chain_support.capabilities.maxImageCount > 0 &&
      image_count > swap_chain_support.capabilities.maxImageCount)
    image_count = swap_chain_support.capabilities.maxImageCount;
  
  Dbg::PrintFunctionInfo(__FUNCTION__, "Swapchain images quantity = ", image_count);

  // Start create info ... again :_:
  VkSwapchainCreateInfoKHR create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  create_info.surface = vk.surface;
  create_info.minImageCount = image_count; 
  create_info.imageFormat = surface_format.format;
  create_info.imageColorSpace = surface_format.colorSpace;
  create_info.imageExtent = extent;
  create_info.imageArrayLayers = 1; // specifies the amount of layers each image consists of
  create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  
  SetQueueFamilies(vk);
  uint32_t queue_family_indices[] = { 
    vk.queue_families.graphics_family, 
    vk.queue_families.present_family
  };

  if ( vk.queue_families.graphics_family != vk.queue_families.present_family ) {
    // It is required for VK_SHARING_MODE_CONCURRENT to specify shared queue families
    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    create_info.queueFamilyIndexCount = 2;
    create_info.pQueueFamilyIndices = queue_family_indices;
    Dbg::PrintFunctionInfo(__FUNCTION__, "Using concurrent sharing mode");
  } else {
    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    // create_info.queueFamilyIndexCount = 0;
    // create_info.pQueueFamilyIndices = nullptr;
    Dbg::PrintFunctionInfo(__FUNCTION__, "Using exclusive sharing mode");
  }

  create_info.preTransform = swap_chain_support.capabilities.currentTransform; // we donot want any transformations
  create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ignore opaque windows
  create_info.presentMode = present_mode;
  create_info.clipped = VK_TRUE; // enable clipping if window is back to other windows
  create_info.oldSwapchain = VK_NULL_HANDLE;

  if ( vkCreateSwapchainKHR(vk.device, &create_info, nullptr, &vk.swap_chain.handle) != VK_SUCCESS )
    throw Except::Swap_Chain_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created swapchain");

  // Retrieve image handles
  vkGetSwapchainImagesKHR(vk.device, vk.swap_chain.handle, &image_count, nullptr);
  vk.swap_chain.images.resize(image_count);
  vkGetSwapchainImagesKHR(vk.device, vk.swap_chain.handle, &image_count, vk.swap_chain.images.data());

  vk.swap_chain.image_format = surface_format.format;
  vk.swap_chain.extent = extent;
}


/// Checks whether swapchains are supported based on passed details
bool App::Impl::CheckSwapChainSupport(const Vulkan_SwapChainSupportDetails& details)
{
  return !details.formats.empty() && !details.present_modes.empty();
}


/// Query for swapchain support of selected device and surface
void App::Impl::QuerySwapChainSupport(Vulkan_SwapChainSupportDetails& details, VkPhysicalDevice dev, VkSurfaceKHR surface)
{
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, surface, &details.capabilities);
  
  uint32_t format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, nullptr);
  if (format_count != 0) 
  {
    details.formats.resize(format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(dev, surface, &format_count, details.formats.data());
  }

  uint32_t present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &present_mode_count, nullptr);
  if (present_mode_count != 0) 
  {
    details.present_modes.resize(present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(dev, surface, &format_count, details.present_modes.data());
  }
}


/// @return the most appropriate format
VkSurfaceFormatKHR App::Impl::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
{
  std::size_t best_candidate_index = 0;
  int best_score = 0;

  for (std::size_t i = 0; i != available_formats.size(); ++i) {
    const auto& available_format = available_formats[i];

    int current_score = Impl::RateSwapSurfaceFormat(available_format);
    if ( current_score > best_score ) {
      best_score = current_score;
      best_candidate_index = i;
    }
  }

  // return just the first format if we haven't found the appropriate one
  return available_formats[best_candidate_index];
}


/// @return score of passed surface format
int App::Impl::RateSwapSurfaceFormat(VkSurfaceFormatKHR sf_format)
{
  int score = 0;

  if ( sf_format.format     == VK_FORMAT_B8G8R8A8_UNORM && 
       sf_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) 
    score = 10;

  return score;
}


///
VkPresentModeKHR App::Impl::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& present_modes)
{
  std::size_t best_candidate_index = 0;
  int best_score = 0;

  for (std::size_t i = 0; i != present_modes.size(); ++i) {
    const VkPresentModeKHR& present_mode = present_modes[i];

    int current_score = Impl::RateSwapPresentMode(present_mode);
    if ( current_score > best_score ) {
      best_score = current_score;
      best_candidate_index = i;
    }
  }

  Dbg::PrintFunctionInfo(__FUNCTION__, "Chosen swapchain present mode (VkPresentModeKHR) = ", present_modes[best_candidate_index]);

  // return just the first format if we haven't found the appropriate one
  return present_modes[best_candidate_index];
}


///
int App::Impl::RateSwapPresentMode(VkPresentModeKHR present_mode)
{
  int score = 0;
  
  switch (present_mode) {
    case VK_PRESENT_MODE_MAILBOX_KHR: score = 100; break;
    case VK_PRESENT_MODE_FIFO_KHR: score = 10; break;
    default: break;
  }

  return score;
}


/// Chooses swap extent (resolution of images inside swap-chain)
/// @return extent of window framebuffer (if capabilities.currentExtent has maximum values of uint32_t)
/// @return capabilities.currentExtent (otherwise)
VkExtent2D App::Impl::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* win)
{
  if ( capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() ) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    
    VkExtent2D actual_extent {
        static_cast<uint32_t>(width), 
        static_cast<uint32_t>(height)
    };
    
    actual_extent.width = std::clamp(
        actual_extent.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
    );

    actual_extent.height = std::clamp(
        actual_extent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
    );

    return actual_extent;
  }
}





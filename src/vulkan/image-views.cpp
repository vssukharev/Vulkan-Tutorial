
#include <cstddef>
#include <string>
#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>


void App::CreateImageViews(Vulkan& vk)
{
  Impl::CreateImageViews(vk.swap_chain, vk.device);
}


void App::Impl::CreateImageViews(Vulkan_SwapChain& sc, VkDevice dev)
{
  sc.image_views.resize(sc.images.size());
  
  for (std::size_t i = 0; i != sc.images.size(); ++i) {
    VkImageViewCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = sc.images[i];
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = sc.image_format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    if ( vkCreateImageView(dev, &create_info, nullptr, &sc.image_views[i]) != VK_SUCCESS )
      throw Except::Image_Views_Creation_Failure{__FUNCTION__};
    Dbg::PrintFunctionInfo(__FUNCTION__, "Created image view for image #", i);
  }
}



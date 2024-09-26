
#include <cstddef>
#include <string>
#include <vulkan/vulkan_core.h>

#include <except.hpp>
#include <debug.hpp>
#include <hello-triangle.hpp>


void App::CreateFramebuffers(
    Container<VkFramebuffer>& framebuffers,
    const Container<VkImageView>& image_views,
    VkDevice logical_device, 
    VkRenderPass render_pass,
    VkExtent2D framebuffer_extent)
{
  framebuffers.resize(image_views.size());

  for (std::size_t i = 0; i != image_views.size(); ++i) {
    VkImageView attachments[] = {
      image_views[i]
    };

    VkFramebufferCreateInfo framebuffer_info {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = render_pass;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = attachments;
    framebuffer_info.width = framebuffer_extent.width;
    framebuffer_info.height = framebuffer_extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(logical_device, &framebuffer_info, nullptr, &framebuffers[i]) != VK_SUCCESS) {
      throw Except::Framebuffer_Creation_Failure{__FUNCTION__};
    }

    Dbg::PrintFunctionInfo(__FUNCTION__, "Created framebuffer #", i);
  }
}




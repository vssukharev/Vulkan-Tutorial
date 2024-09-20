
#include <cstddef>
#include <string>
#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <except.hpp>
#include <debug.hpp>

void App::CreateFramebuffers(Vulkan& vk)
{
  vk.swap_chain.framebuffers.resize(vk.swap_chain.image_views.size());

  for (std::size_t i = 0; i != vk.swap_chain.image_views.size(); ++i) {
    VkImageView attachments[] = {
      vk.swap_chain.image_views[i]
    };

    VkFramebufferCreateInfo framebuffer_info {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = vk.render_pass;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = attachments;
    framebuffer_info.width = vk.swap_chain.extent.width;
    framebuffer_info.height = vk.swap_chain.extent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(vk.device, &framebuffer_info, nullptr, &vk.swap_chain.framebuffers[i]) != VK_SUCCESS) {
      std::string err_msg = "in " + std::string{__FUNCTION__} + " #" + std::to_string(i);
      throw Except::Framebuffer_Creation_Failure{err_msg.c_str()};
    }

    Dbg::PrintFunctionInfo(__FUNCTION__, "Created framebuffer #", i);
  }
}




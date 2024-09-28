
#include <GLFW/glfw3.h>
#include <cstddef>
#include <string>
#include <vulkan/vulkan_core.h>

#include "decl.hpp"
#include <except.hpp>
#include <debug.hpp>
#include <hello-triangle.hpp>


///
void App::CreateFramebuffers(
    Framebuffers& rFramebuffers,
    const ImageViews& imageViews,
    VkDevice logicalDevice, 
    VkRenderPass renderPass,
    VkExtent2D framebufferExtent)
{
  rFramebuffers.resize(imageViews.size());

  for (std::size_t i = 0; i != imageViews.size(); ++i) {
    VkImageView attachments[] = {
      imageViews[i]
    };

    VkFramebufferCreateInfo framebuffer_info {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = renderPass;
    framebuffer_info.attachmentCount = 1;
    framebuffer_info.pAttachments = attachments;
    framebuffer_info.width = framebufferExtent.width;
    framebuffer_info.height = framebufferExtent.height;
    framebuffer_info.layers = 1;

    if (vkCreateFramebuffer(logicalDevice, &framebuffer_info, nullptr, &rFramebuffers[i]) != VK_SUCCESS) {
      throw Except::Framebuffer_Creation_Failure{__FUNCTION__};
    }

    Dbg::PrintFunctionInfo(__FUNCTION__, "Created framebuffer #", i);
  }
}


///
void App::FramebufferResizeCallback(
    GLFWwindow* window, 
    int         width, 
    int         height)
{
  auto& swap_chain_state = *reinterpret_cast<SWAPCHAIN_STATE_BITS_T*>(
      glfwGetWindowUserPointer(window));
  swap_chain_state |= SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED;
}


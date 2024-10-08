
#include <except.hpp>
#include <debug.hpp>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>

///
void App::CreateRenderPass(
    VkRenderPass& render_pass,
    VkFormat image_format,
    VkDevice logical_device)
{
  VkAttachmentDescription color_attachment {};
  color_attachment.format = image_format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; 
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; 
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  // loadOp - is what to do with data before rendering
  // VK_ATTACHMENT_LOAD_OP_LOAD - preserve the existing contents
  // VK_ATTACHMENT_LOAD_OP_CLEAR - clear the values to a constant
  // VK_ATTACHMENT_LOAD_OP_DONT_CARE - existing contents are undefined; we don't caare about them
  // storeOp - is what to do with data after rendering
  // VK_ATTACHMENT_STORE_OP_STORE - rendered contents will be stored in memory and can be read later
  // VK_ATTACHMENT_STORE_OP_DONT_CARE - contents of the framebuffer will be undefined after rendering operation
  // initialLayout - image layout before rendering
  // finalLayout - image layout when render pass finishes
  // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL - images are used as color attachments
  // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR - images are presented in the swap chain
  // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL - images are used as destination for a memory copy operation


  VkAttachmentReference color_attachment_ref {};
  color_attachment_ref.attachment = 0;
  // We want subpass to use this layout while rendering attachment under index above
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref; // The index of the attachment in this array is directly referenced from the fragment shader with the 'layout(location = 0) out vec4 outColor' directive in fragment shader
  // We can also specify the other types of attachments
  // pInputAttachments - attachments that are read from shader
  // pResolveAttachments - attachments used for multisampling color attachments
  // pDepthStencilAttachment - attachment for depth and stencil data
  // pPreserveAttachments - attachments that are not used by this subpass, but for which the data must be preserved

  // ????????
  VkSubpassDependency dependency {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;

  VkRenderPassCreateInfo render_pass_info {};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;
  render_pass_info.dependencyCount = 1;
  render_pass_info.pDependencies = &dependency;

  if (vkCreateRenderPass(logical_device, &render_pass_info, nullptr, &render_pass) != VK_SUCCESS)
    throw Except::Render_Pass_Creation_Failure{__FUNCTION__};
  
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created render pass");
}


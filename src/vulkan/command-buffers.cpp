
#include "hello-triangle.hpp"
#include <vulkan/vulkan_core.h>

#include <implementation.hpp>
#include <debug.hpp>
#include <except.hpp>


///
void App::CreateCommandBuffer(Vulkan& vk)
{
  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = vk.command_pool;
  // level can be:
  // VK_COMMAND_BUFFER_LEVEL_PRIMARY - can be submitted to a queue for execution, but cannot be called from other command buffers
  // VK_COMMAND_BUFFER_LEVEL_SECONDARY - cannot be submited directly, but can be called from primary command buffers
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = 1;

  if ( vkAllocateCommandBuffers(vk.device, &alloc_info, &vk.command_buffer) != VK_SUCCESS )
    throw Except::Command_Buffer_Allocation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated command buffer");
}


///
void App::RecordCommandBuffer(Vulkan& vk, uint32_t image_index)
{
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  // flags can be:
  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT - the command buffer will be rerecorded right after executing it once
  // VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT - this is a secondary command buffer that will be entirely within a single render pass
  // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT - the command buffer can be resubmitted while it is also already pending execution

  if ( vkBeginCommandBuffer(vk.command_buffer, &begin_info) != VK_SUCCESS )
    throw Except::Begin_Command_Buffer_Failure{__FUNCTION__};
  // Dbg::PrintFunctionInfo(__FUNCTION__, "Started recording command buffer");

  VkRenderPassBeginInfo render_pass_info {};
  render_pass_info.sType  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = vk.render_pass;
  render_pass_info.framebuffer = vk.swap_chain.framebuffers[image_index];
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = vk.swap_chain.extent;
  
  VkClearValue clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(vk.command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(vk.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk.graphics_pipeline);

  VkViewport viewport {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(vk.swap_chain.extent.width);
  viewport.height = static_cast<float>(vk.swap_chain.extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(vk.command_buffer, 0, 1, &viewport);

  VkRect2D scissor {};
  scissor.offset = {0, 0};
  scissor.extent = vk.swap_chain.extent;
  vkCmdSetScissor(vk.command_buffer, 0, 1, &scissor);

  // 2nd - vertexCount - we have 3 vertices to draw
  // 3rd - instanceCount - used for instanced rendering, set 1 if we donot care
  // 4th - firstVertex - defines the lowest value of gl_VertexIndex in vertex shader
  // 5th - firstInstance - difines the lowest value of gl_InstanceIndex in vertex shader
  vkCmdDraw(vk.command_buffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(vk.command_buffer);

  if ( vkEndCommandBuffer(vk.command_buffer) != VK_SUCCESS )
    throw Except::Record_Command_Buffer_Failure{__FUNCTION__};
}




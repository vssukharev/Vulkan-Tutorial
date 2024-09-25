
#include <cstdint>

#include <debug.hpp>
#include <except.hpp>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>

///
void App::CreateCommandBuffers(
    Container<VkCommandBuffer>& command_buffers,
    VkDevice logical_device,
    VkCommandPool command_pool)
{
  command_buffers.reallocate(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = command_pool;
  // level can be:
  // VK_COMMAND_BUFFER_LEVEL_PRIMARY - can be submitted to a queue for execution, but cannot be called from other command buffers
  // VK_COMMAND_BUFFER_LEVEL_SECONDARY - cannot be submited directly, but can be called from primary command buffers
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = command_buffers.size();

  if ( vkAllocateCommandBuffers(logical_device, &alloc_info, command_buffers.data()) != VK_SUCCESS )
    throw Except::Command_Buffer_Allocation_Failure{__FUNCTION__};

  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated command ",  command_buffers.size(), " buffers");
}


///
void App::RecordCommandBuffer(
    VkCommandBuffer& command_buffer,
    VkFramebuffer framebuffer,
    VkExtent2D image_extent,
    VkRenderPass render_pass,
    VkPipeline graphics_pipeline)
{
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  // flags can be:
  // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT - the command buffer will be rerecorded right after executing it once
  // VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT - this is a secondary command buffer that will be entirely within a single render pass
  // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT - the command buffer can be resubmitted while it is also already pending execution

  if ( vkBeginCommandBuffer(command_buffer, &begin_info) != VK_SUCCESS )
    throw Except::Begin_Command_Buffer_Failure{__FUNCTION__};
  // Dbg::PrintFunctionInfo(__FUNCTION__, "Started recording command buffer");

  VkRenderPassBeginInfo render_pass_info {};
  render_pass_info.sType  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = render_pass;
  render_pass_info.framebuffer = framebuffer;
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = image_extent;
  
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

  VkViewport viewport {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(image_extent.width);
  viewport.height = static_cast<float>(image_extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer, 0, 1, &viewport);

  VkRect2D scissor {};
  scissor.offset = {0, 0};
  scissor.extent = image_extent;
  vkCmdSetScissor(command_buffer, 0, 1, &scissor);

  // 2nd - vertexCount - we have 3 vertices to draw
  // 3rd - instanceCount - used for instanced rendering, set 1 if we donot care
  // 4th - firstVertex - defines the lowest value of gl_VertexIndex in vertex shader
  // 5th - firstInstance - difines the lowest value of gl_InstanceIndex in vertex shader
  vkCmdDraw(command_buffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(command_buffer);

  if ( vkEndCommandBuffer(command_buffer) != VK_SUCCESS )
    throw Except::Record_Command_Buffer_Failure{__FUNCTION__};
}




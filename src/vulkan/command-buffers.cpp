
#include "config.hpp"
#include <cstdint>

#include <debug.hpp>
#include <except.hpp>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>

///
void App::CreateCommandBuffers(
    CommandBuffers&     rCommandBuffers,
    VkDevice            logicalDevice,
    const CommandPools& commandPools)
{
  rCommandBuffers.graphics.resize(MAX_FRAMES_IN_FLIGHT);
  rCommandBuffers.transfer.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo graphics_alloc_info {};
  graphics_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  graphics_alloc_info.commandPool = commandPools.graphics;
  graphics_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  graphics_alloc_info.commandBufferCount = rCommandBuffers.graphics.size();

  if ( vkAllocateCommandBuffers(logicalDevice, &graphics_alloc_info, rCommandBuffers.graphics.data()) != VK_SUCCESS )
    throw Except::Command_Buffer_Allocation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated ",  rCommandBuffers.graphics.size(), " graphics command buffers");
 

  VkCommandBufferAllocateInfo transfer_alloc_info {};
  transfer_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  transfer_alloc_info.commandPool = commandPools.transfer;
  transfer_alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  transfer_alloc_info.commandBufferCount = rCommandBuffers.transfer.size();

  if ( vkAllocateCommandBuffers(logicalDevice, &transfer_alloc_info, rCommandBuffers.transfer.data()) != VK_SUCCESS )
    throw Except::Command_Buffer_Allocation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated ",  rCommandBuffers.transfer.size(), " transfer command buffers");
}


///
void App::RecordCommandBuffer(
    VkCommandBuffer&  rCommandBuffer,
    const Vertices&   vertices,
    VkFramebuffer     framebuffer,
    VkExtent2D        imageExtent,
    VkRenderPass      renderPass,
    VkPipeline        graphicsPipeline,
    VkBuffer          vertexBuffer)
{
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if ( vkBeginCommandBuffer(rCommandBuffer, &begin_info) != VK_SUCCESS )
    throw Except::Begin_Command_Buffer_Failure{__FUNCTION__};
  // Dbg::PrintFunctionInfo(__FUNCTION__, "Started recording command buffer");

  VkRenderPassBeginInfo render_pass_info {};
  render_pass_info.sType  = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass = renderPass;
  render_pass_info.framebuffer = framebuffer;
  render_pass_info.renderArea.offset = {0, 0};
  render_pass_info.renderArea.extent = imageExtent;
  
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  render_pass_info.clearValueCount = 1;
  render_pass_info.pClearValues = &clear_color;

  vkCmdBeginRenderPass(rCommandBuffer, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(rCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
  
  VkBuffer vertex_buffers[] = { vertexBuffer };
  VkDeviceSize offsets[] = { 0 };
  vkCmdBindVertexBuffers(rCommandBuffer, 0, 1, vertex_buffers, offsets);

  VkViewport viewport {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(imageExtent.width);
  viewport.height = static_cast<float>(imageExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(rCommandBuffer, 0, 1, &viewport);

  VkRect2D scissor {};
  scissor.offset = {0, 0};
  scissor.extent = imageExtent;
  vkCmdSetScissor(rCommandBuffer, 0, 1, &scissor);

  // 2nd - vertexCount - we have 3 vertices to draw
  // 3rd - instanceCount - used for instanced rendering, set 1 if we donot care
  // 4th - firstVertex - defines the lowest value of gl_VertexIndex in vertex shader
  // 5th - firstInstance - difines the lowest value of gl_InstanceIndex in vertex shader
  vkCmdDraw(rCommandBuffer, vertices.size(), 1, 0, 0);

  vkCmdEndRenderPass(rCommandBuffer);

  if ( vkEndCommandBuffer(rCommandBuffer) != VK_SUCCESS )
    throw Except::Record_Command_Buffer_Failure{__FUNCTION__};
}




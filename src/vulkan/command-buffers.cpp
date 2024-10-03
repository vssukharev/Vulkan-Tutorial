
#include <cstdint>

#include <debug.hpp>
#include <except.hpp>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>

///
void App::CreateCommandBuffers(
    CommandBuffers& rCommandBuffers,
    VkDevice        logicalDevice,
    VkCommandPool   commandPool)
{
  rCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool = commandPool;
  // level can be:
  // VK_COMMAND_BUFFER_LEVEL_PRIMARY - can be submitted to a queue for execution, but cannot be called from other command buffers
  // VK_COMMAND_BUFFER_LEVEL_SECONDARY - cannot be submited directly, but can be called from primary command buffers
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount = rCommandBuffers.size();

  if ( vkAllocateCommandBuffers(logicalDevice, &alloc_info, rCommandBuffers.data()) != VK_SUCCESS )
    throw Except::Command_Buffer_Allocation_Failure{__FUNCTION__};

  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated command ",  rCommandBuffers.size(), " buffers");
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




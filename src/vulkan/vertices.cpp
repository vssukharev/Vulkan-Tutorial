
#include "debug.hpp"
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan_core.h>

#include <decl.hpp>
#include <hello-triangle.hpp>
#include <except.hpp>

///
void App::CreateVertexBuffer(
    VkBuffer&             rVertBuffer,
    VkDeviceMemory&       rVertBufferMemory,
    const Vertices&       vertices,
    const QueueFamilies&  queueFamilies,
    VkQueue               submitQueue,
    VkDevice              logicalDevice,
    VkPhysicalDevice      physicalDevice,
    VkCommandPool         commandPool)
{
  VkDeviceSize buffer_size = sizeof(Vertex) * vertices.size();

  VkBuffer staging_buffer;
  VkDeviceMemory staging_buffer_mem;
  
  CreateBuffer(
    staging_buffer, staging_buffer_mem, buffer_size, 
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
    queueFamilies, logicalDevice, physicalDevice);

  void* buffer_data;
  vkMapMemory(logicalDevice, staging_buffer_mem, 0, buffer_size, 0, &buffer_data);
  memcpy(buffer_data, vertices.data(), buffer_size);
  vkUnmapMemory(logicalDevice, staging_buffer_mem);

  CreateBuffer(
      rVertBuffer, rVertBufferMemory, buffer_size, 
      VK_BUFFER_USAGE_TRANSFER_DST_BIT |
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
      queueFamilies, logicalDevice, physicalDevice);

  CopyBuffer(
      staging_buffer, 
      rVertBuffer,
      buffer_size, 
      commandPool, 
      submitQueue, 
      logicalDevice);

  vkDestroyBuffer(logicalDevice, staging_buffer, nullptr);
  vkFreeMemory(logicalDevice, staging_buffer_mem, nullptr);
}


///
void App::SetVertices(
    Vertices& rVertices)
{
  rVertices = {
    { { 0,   -0.5 }, { 1.0, 0.0, 0.0 } },
    { { 0.5,  0.5 }, { 0.0, 1.0, 0.0 } },
    { { -0.5, 0.5 }, { 0.0, 0.0, 1.0 } },
  };
}


///
VkVertexInputBindingDescription 
  App::GetVertexBindingDescription()
{
  VkVertexInputBindingDescription binding_description {};
  // binding - index of the binding in the array of bindings
  binding_description.binding = 0;
  binding_description.stride = sizeof(Vertex);
  binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return binding_description;
}


///
std::array<VkVertexInputAttributeDescription, 2> 
  App::GetVertexAttributeDescriptions()
{
  std::array<VkVertexInputAttributeDescription, 2> attr_descriptions {};
  attr_descriptions[0].binding = 0;
  attr_descriptions[0].location = 0;
  attr_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attr_descriptions[0].offset = offsetof(Vertex, pos);

  attr_descriptions[1].binding = 0;
  attr_descriptions[1].location = 1;
  attr_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attr_descriptions[1].offset = offsetof(Vertex, color);

  return attr_descriptions;
}


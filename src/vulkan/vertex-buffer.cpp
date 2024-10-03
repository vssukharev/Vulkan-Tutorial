
#include <cstdint>
#include <cstring>
#include <vulkan/vulkan_core.h>

#include <decl.hpp>
#include <hello-triangle.hpp>
#include <except.hpp>

///
void App::CreateVertexBuffer(
    VkBuffer&        rBuffer,
    VkDeviceMemory&  rBufferMemory,
    const Vertices&  vertices,
    VkDevice         logicalDevice,
    VkPhysicalDevice physicalDevice)
{
  VkBufferCreateInfo buffer_info {};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = sizeof(Vertex) * vertices.size();
  buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(logicalDevice, &buffer_info, nullptr, &rBuffer) != VK_SUCCESS)
    throw Except::Buffer_Creation_Failure{__FUNCTION__};

  VkMemoryRequirements mem_requirements;
  vkGetBufferMemoryRequirements(logicalDevice, rBuffer, &mem_requirements);

  VkMemoryAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_requirements.size;
  alloc_info.memoryTypeIndex = FindMemoryType(
    physicalDevice,
    mem_requirements.memoryTypeBits,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(logicalDevice, &alloc_info, nullptr, &rBufferMemory) != VK_SUCCESS)
    throw Except::Buffer_Memory_Allocation_Failure{__FUNCTION__};

  vkBindBufferMemory(logicalDevice, rBuffer, rBufferMemory, 0);

  void* buffer_data;
  vkMapMemory(logicalDevice, rBufferMemory, 0, buffer_info.size, 0, &buffer_data);
  memcpy(buffer_data, vertices.data(), buffer_info.size);
  vkUnmapMemory(logicalDevice, rBufferMemory);
}


///
uint32_t App::FindMemoryType(
    VkPhysicalDevice      physicalDevice,
    uint32_t              typeFilter,
    VkMemoryPropertyFlags properties)
{
  VkPhysicalDeviceMemoryProperties mem_properties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &mem_properties);


  for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i)
    if ((typeFilter & (1 << i)) && 
        (mem_properties.memoryTypes[i].propertyFlags & properties))
      return i;

  throw Except::Memory_Type_Missing{__FUNCTION__};
}



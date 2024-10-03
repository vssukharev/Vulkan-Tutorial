
#include <glm/vector_relational.hpp>
#include <hello-triangle.hpp>
#include <except.hpp>
#include <debug.hpp>
#include <vulkan/vulkan_core.h>

/// TODO: lower dependencies through turning queueFamilies into something other
void App::CreateBuffer(
    VkBuffer&             rBuffer,
    VkDeviceMemory&       rBufferMemory,
    VkDeviceSize          bufferSize,
    VkBufferUsageFlags    bufferUsage,
    VkMemoryPropertyFlags memoryProperties,
    const QueueFamilies&  queueFamilies,
    VkDevice              logicalDevice,
    VkPhysicalDevice      physicalDevice)
{
  VkBufferCreateInfo buffer_info {};
  buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_info.size = bufferSize;
  buffer_info.usage = bufferUsage;
  buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
  uint32_t queue_family_indices[] = { 
    queueFamilies.graphics, 
    queueFamilies.transfer, 
  };
  buffer_info.queueFamilyIndexCount = 2;
  buffer_info.pQueueFamilyIndices = queue_family_indices;

  if (vkCreateBuffer(logicalDevice, &buffer_info, nullptr, &rBuffer) != VK_SUCCESS)
    throw Except::Buffer_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created buffer with usage = ", bufferUsage);

  VkMemoryRequirements mem_requirements;
  vkGetBufferMemoryRequirements(logicalDevice, rBuffer, &mem_requirements);

  VkMemoryAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.allocationSize = mem_requirements.size;
  alloc_info.memoryTypeIndex = FindMemoryType(physicalDevice, mem_requirements.memoryTypeBits, memoryProperties);

  if (vkAllocateMemory(logicalDevice, &alloc_info, nullptr, &rBufferMemory) != VK_SUCCESS)
    throw Except::Buffer_Memory_Allocation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Allocated buffer");

  vkBindBufferMemory(logicalDevice, rBuffer, rBufferMemory, 0);
}


void App::CopyBuffer(
    VkBuffer      srcBuffer,
    VkBuffer      dstBuffer,
    VkDeviceSize  buffersSize,
    VkCommandPool commandPool,
    VkQueue       submitQueue,
    VkDevice      logicalDevice)
{
  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandPool = commandPool;
  alloc_info.commandBufferCount = 1;
    
  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(logicalDevice, &alloc_info, &command_buffer);

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(command_buffer, &begin_info);
  VkBufferCopy copy_region {};
  copy_region.srcOffset = 0;
  copy_region.dstOffset = 0;
  copy_region.size = buffersSize;
  vkCmdCopyBuffer(command_buffer, srcBuffer, dstBuffer, 1, &copy_region);
  vkEndCommandBuffer(command_buffer);

  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &command_buffer;

  vkQueueSubmit(submitQueue, 1, &submit_info, VK_NULL_HANDLE);
  vkQueueWaitIdle(submitQueue);

  vkFreeCommandBuffers(logicalDevice, commandPool, 1, &command_buffer);
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



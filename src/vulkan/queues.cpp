
#include "hello-triangle.hpp"
#include <set>

#include <implementation.hpp>
#include <debug.hpp>

/// Finds and sets queue families for vk.physical_device (safe version)
/// Operation not performed if queue families are already set
/// NOTE: not really sure should it be called only once or every time
void App::SetQueueFamilies(Vulkan& vk)
{
  if ( !Impl::CheckQueueFamiliesSupport(vk.queue_families) )
    Impl::QueryQueueFamilies(vk.queue_families, vk.physical_device, vk.surface);
}



/// @return std::vector of Vulkan queues create infos
std::vector<VkDeviceQueueCreateInfo> App::Impl::GetVulkanQueueCreateInfos(Vulkan_QueueFamilies& qf)
{
  std::vector<VkDeviceQueueCreateInfo> queue_create_infos {};
  
  std::set<uint32_t> unique_queue_families { 
    qf.graphics_family, 
    qf.present_family 
  };

  float queue_priority = 1.0f;
  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  return queue_create_infos;
}


/// WARN: we can check for queue families only after setting Vulkan_Queue_Families::present_families
/// @return Validness of queue families
bool App::Impl::CheckQueueFamiliesSupport(const Vulkan_QueueFamilies& qf)
{
  return qf.supported_families & QUEUE_FAMILIES_BITS::GRAPHICS &&
         qf.supported_families & QUEUE_FAMILIES_BITS::PRESENTATION;
}


/// Finds queue families for entered physical device
/// NOTE: can be called multiple times without performance losses
void App::Impl::QueryQueueFamilies(Vulkan_QueueFamilies& qf, VkPhysicalDevice dev, VkSurfaceKHR surface)
{
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, nullptr);
  std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(dev, &queue_family_count, queue_families.data());

  uint32_t family_index = 0;
  for (const auto& queue_family : queue_families)
  {
    if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      qf.supported_families |= QUEUE_FAMILIES_BITS::GRAPHICS;
      qf.graphics_family = family_index;
    }
    
    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(dev, family_index, surface, &present_support);
    if (present_support) {
      qf.supported_families |= QUEUE_FAMILIES_BITS::PRESENTATION;
      qf.present_family = family_index;
    }

    ++family_index;
  }

  Dbg::PrintFunctionInfo(__FUNCTION__, "Graphics family = ", qf.graphics_family);
  Dbg::PrintFunctionInfo(__FUNCTION__, "Present family = ", qf.present_family);
}



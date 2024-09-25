
#include "hello-triangle.hpp"
#include <set>

#include <hello-triangle.hpp>
#include <debug.hpp>
#include <vulkan/vulkan_core.h>


///
void App::SetQueueFamilies(QueueFamilies& qf, VkPhysicalDevice physical_dev, VkSurfaceKHR surface)
{
  if ( !CheckQueueFamiliesSupport(qf) )
    QueryQueueFamilies(qf, physical_dev, surface);
}

void App::SetQueues(Queues& queues, const QueueFamilies& qf, VkDevice logical_device)
{
  vkGetDeviceQueue(logical_device, qf.graphics_family, 0, &queues.graphics_queue);
  vkGetDeviceQueue(logical_device, qf.present_family, 0, &queues.presentation_queue);
}

/// WARN: we can check for queue families only after setting Vulkan_Queue_Families::present_families
/// @return Validness of queue families
bool App::CheckQueueFamiliesSupport(const QueueFamilies& qf)
{
  return qf.supported_families & QUEUE_FAMILIES_BITS::GRAPHICS &&
         qf.supported_families & QUEUE_FAMILIES_BITS::PRESENTATION;
}


/// Finds queue families for entered physical device
/// NOTE: can be called multiple times without performance losses
void App::QueryQueueFamilies(QueueFamilies& qf, VkPhysicalDevice dev, VkSurfaceKHR surface)
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



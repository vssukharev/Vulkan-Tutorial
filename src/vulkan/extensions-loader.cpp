
#include <vulkan/vulkan_core.h>

// ------ Macros ------

#define RETURN_LOADER(NAME, ...) \
  auto func = (PFN_##NAME) vkGetInstanceProcAddr(instance, #NAME); \
  if (func != nullptr) \
    return func(__VA_ARGS__); \
  else \
    return VK_ERROR_EXTENSION_NOT_PRESENT;


#define NORETURN_LOADER(NAME, ...) \
  auto func = (PFN_##NAME) vkGetInstanceProcAddr(instance, #NAME); \
  if (func != nullptr) \
    return func(__VA_ARGS__); \

// --------------------


// ------ Functions ------

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDebugUtilsMessengerEXT*                   pMessenger)
{
  RETURN_LOADER(vkCreateDebugUtilsMessengerEXT, instance, pCreateInfo, pAllocator, pMessenger);
}


VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(
    VkInstance                                  instance,
    VkDebugUtilsMessengerEXT                    messenger,
    const VkAllocationCallbacks*                pAllocator)
{
  NORETURN_LOADER(vkDestroyDebugUtilsMessengerEXT, instance, messenger, pAllocator);
}

// -----------------------


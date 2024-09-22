
#include <vector>
#include <cstring>
#include <iostream>

#include <except.hpp>
#include <implementation.hpp>
#include <debug.hpp>


/// Checks for validation layers availability
/// @throw Except::Val_Layers_Missing
void App::Dbg::CheckValidationLayersSupport()
{
#ifndef NDEBUG
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  std::vector<const char*> unavailable_val_layers {};

  for (const char* layer_name : gl_validation_layers) {
    bool layer_found = false;

    for (const auto& available_layer : available_layers) {
      Dbg::PrintFunctionInfo(__FUNCTION__, "Available layer = ", available_layer.layerName);
      if ( strcmp(layer_name, available_layer.layerName) == 0 ) {
        layer_found = true;
        break;
      }
    }

    if (!layer_found) {
      unavailable_val_layers.push_back(layer_name);
    }
  }

  if (unavailable_val_layers.size() != 0)
    throw Except::Val_Layers_Missing{unavailable_val_layers[0]};
#endif // !NDEBUG
}



/// @throw Except::Instance_Failure
void App::Dbg::CreateDebugMessenger(Vulkan& vk)
{
#ifndef NDEBUG
  
  auto create_info = Impl::GenerateDebugMessengerCreateInfo();

  // We are allowed to use vk.dbg.messenger as we are inside '#ifndef NDEBUG' block
  if (vkCreateDebugUtilsMessengerEXT(vk.instance, &create_info, nullptr, &vk.dbg.messenger) != VK_SUCCESS)
    throw Except::Debug_Messenger_Creation_Failure{__FUNCTION__};
  Dbg::PrintFunctionInfo(__FUNCTION__, "Created debug messenger");

#endif // !NDEBUG
}



/// Destroys debug messenger
void App::Dbg::DestroyDebugMessenger(Vulkan& vk) noexcept
{
#ifndef NDEBUG
  vkDestroyDebugUtilsMessengerEXT(vk.instance, vk.dbg.messenger, nullptr);
#endif // !NDEBUG
}



/// Generates Vulkan prefix for Vulkan logs
std::string App::Dbg::Impl::GenerateVulkanLogPrefix(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type) noexcept
{
  std::string res {};

#ifndef NDEBUG
  if ( message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT ) res += VLK_VERB;
  if ( message_severity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT ) res += VLK_INFO;
  if ( message_severity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ) res += VLK_WARN;
  if ( message_severity &  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT ) res += VLK_ERRO;
  res += ' ';
  if ( message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT ) res += VLK_LAYERS_GENERAL;
  if ( message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT ) res += VLK_LAYERS_VALIDATION;
  if ( message_type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT ) res += VLK_LAYERS_PERFORMANCE;
  res += " : ";
#endif // !NDEBUG

  return res;
}



/// Returns a boolean that indicates if the Vulkan call triggered the validation layer message should be aborted.
VKAPI_ATTR VkBool32 VKAPI_CALL App::Dbg::Impl::VulkanCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_type,
      const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
      void* user_data)
{
#ifndef NDEBUG
  std::cerr << GenerateVulkanLogPrefix(message_severity, message_type) << callback_data->pMessage << '\n';
#endif // !NDEBUG

  return VK_FALSE;
}



/// Generated create_info for Vulkan debug messengerr
VkDebugUtilsMessengerCreateInfoEXT App::Dbg::Impl::GenerateDebugMessengerCreateInfo() noexcept
{
  VkDebugUtilsMessengerCreateInfoEXT create_info {};
  
#ifndef NDEBUG
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = Dbg::Impl::VulkanCallback;
  create_info.pUserData = nullptr; // optional
#endif // !NDEBUG

  return create_info;
}




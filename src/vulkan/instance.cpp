
#include <implementation.hpp>
#include <debug.hpp>
#include <except.hpp>

/// @throw Except::Instance_Failure
void App::CreateInstance(Vulkan& vk)
{
  // exception is thrown if requested validation layers are not found
  Dbg::CheckValidationLayersSupport();

  // fill our application with info
  VkApplicationInfo app_info {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Hello Triangle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  // create inforamtion for Vulkan instance
  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  
  std::vector<const char*> extensions = Impl::GetRequiredVulkanExtensions();
  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();

  // validation layers (validation_layers vector size equals zero and data is nullptr if NDEBUG is defined)
  create_info.enabledLayerCount = Dbg::gl_validation_layers.size();
  create_info.ppEnabledLayerNames = Dbg::gl_validation_layers.data();

#ifndef NDEBUG
  auto debug_create_info = Dbg::Impl::GenerateDebugMessengerCreateInfo();
  create_info.pNext = &debug_create_info;
#endif // !NDEBUG

  // dump Vulkan extensions
  Dbg::PrintRequiredVulkanExtensions();
  // Dbg::PrintAvailableVulkanExtensions();

  if ( vkCreateInstance(&create_info, nullptr, &vk.instance) != VK_SUCCESS )
    throw Except::Instance_Creation_Failure{__FUNCTION__};
}


///
std::vector<const char*> App::Impl::GetRequiredVulkanExtensions() noexcept
{
  uint32_t glfw_extension_count = 0;
  const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
  
  std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

#ifndef NDEBUG
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // !NDEBUG

  return extensions; 
}




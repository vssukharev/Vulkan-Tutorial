
#include <iostream>

#include <implementation.hpp>
#include <debug.hpp>
#include <config.hpp>

///
void App::Dbg::PrintAvailableVulkanExtensions() noexcept
{
#ifndef NDEBUG
  uint32_t ext_count = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, nullptr);
  std::vector<VkExtensionProperties> extensions (ext_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &ext_count, extensions.data());

  std::cout << Dbg::VLK_INFO << " Available Vulkan extensions:\n";
  for (const auto& ext : extensions)
  {
    std::cout << INDENT << ext.extensionName << '\n';
  }
#endif // !NDEBUG
}


///
void App::Dbg::PrintRequiredVulkanExtensions() noexcept
{
#ifndef NDEBUG
  uint32_t ext_count = 0;
  const char** extensions = glfwGetRequiredInstanceExtensions(&ext_count);
  
  std::cout << Dbg::VLK_INFO << " Required Vulkan extensions:\n";
  for (uint32_t i = 0; i != ext_count; ++i)
  {
    std::cout << INDENT << extensions[i] << '\n';
  }
#endif // !NDEBUG
}


///
// template <typename... Types>
// void App::Dbg::Impl::PrintFunctionInfo(const char* func_name, Types... args) noexcept
// {
//   ( ( ( std::cout << Dbg::VLK_VERB << '(' << __PRETTY_FUNCTION__ << ") : " ) << args << ", " ), ... );
// }


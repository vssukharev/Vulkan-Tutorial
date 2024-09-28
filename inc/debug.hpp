
#pragma once

#include "config.hpp"
#include <string>

#include <hello-triangle.hpp>
#include <iostream>

// NOTE: all functions inside Dbg namespace do nothing if NDEBUG is defined
namespace App::Dbg {

  // ------ Vulkan ------
  // --- API
  void PrintRequiredVulkanExtensions() noexcept;
  void PrintAvailableVulkanExtensions() noexcept;
  void CheckValidationLayersSupport();
  
  void CreateDebugMessenger(
      VkDebugUtilsMessengerEXT& rDebugMessenger, 
      VkInstance instance);

  void DestroyDebugMessenger(
      VkDebugUtilsMessengerEXT& rDebugMessenger, 
      VkInstance instance) noexcept;


  template <typename... Types>
  void PrintFunctionInfo(const char* func_name, const Types&... args) noexcept
  {
#ifndef NDEBUG
    std::cout << Dbg::VLK_VERB << " " << COL_ORANGE << func_name << COL_NO << " : ";
    ( (std::cout << args), ...  );
    std::cout << '\n';
    std::cout.flush();
#endif // !NDEBUG
  }
  
  // --- Implementation
  namespace Impl {

    std::string GenerateVulkanLogPrefix(
          VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
          VkDebugUtilsMessageTypeFlagsEXT message_type) noexcept;
    
    // TODO: rename to DebugMessengerCallback
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanCallback(
          VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
          VkDebugUtilsMessageTypeFlagsEXT message_type,
          const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
          void* user_data);
    
    VkDebugUtilsMessengerCreateInfoEXT GenerateDebugMessengerCreateInfo() noexcept;
  }
  // --------------------
  
}



#pragma once

#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

// --- Constants ---
#define COL_RED "\033[0;31m"
#define COL_GREEN "\033[0;32m"
#define COL_ORANGE "\033[38;2;255;165;0m"
#define COL_NAVY "\033[38;2;0;0;128m"
#define COL_LIGHTBLUE "\033[38;2;173;216;230m"
#define COL_BROWN "\033[38;2;214;76;76m"
#define COL_SALAD "\033[38;2;148;226;213m"
#define COL_NO "\033[0m"


namespace App {
  const std::vector<const char*> gl_device_extensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
  };

  constexpr const char* PROJECT_NAME = "hello-triangle";
  constexpr const char* SHADERS_DIR = "src/shaders";
}

namespace App::Dbg {
  constexpr const char* LINE = "--------------------------------------\n";
  constexpr const char* INDENT = "    ";

  constexpr const char* VLK_VERB = COL_GREEN "[VLK_VERB]" COL_NO;
  constexpr const char* VLK_INFO = COL_LIGHTBLUE "[VLK_INFO]" COL_NO;
  constexpr const char* VLK_WARN = COL_ORANGE "[VLK_WARN]" COL_NO;
  constexpr const char* VLK_ERRO = COL_RED "[VLK_ERRO]" COL_NO;
  
  constexpr const char* VLK_LAYERS_GENERAL = COL_GREEN "(GENERAL)" COL_NO;
  constexpr const char* VLK_LAYERS_VALIDATION = COL_LIGHTBLUE "(VALIDATION)" COL_NO;
  constexpr const char* VLK_LAYERS_PERFORMANCE = COL_ORANGE "(PERFORMANCE)" COL_NO;


  const std::vector<const char*> gl_validation_layers {
#ifndef NDEBUG
    "VK_LAYER_KHRONOS_validation",
    // "VK_LAYER_LUNARG_api_dump"
#endif // !NDEBUG
  };
}



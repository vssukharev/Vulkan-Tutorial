  
#include <cstdint>
#include <helpers/mini-vec.hpp>
#include <type_traits>
#include <vector>

#pragma once

namespace App {
  // --- Enums
  enum QUEUE_FAMILIES_BITS : uint8_t
  {
    QUEUE_FAMILY_GRAPHICS     = 0x1,
    QUEUE_FAMILY_PRESENTATION = 0x2,
    QUEUE_FAMILY_TRANSFER     = 0x4,
  };
  using QUEUE_FAMILIES_BITS_T = std::underlying_type_t<QUEUE_FAMILIES_BITS>;


  enum SWAPCHAIN_STATE_BITS : uint8_t
  {
    SWAPCHAIN_STATE_FRAMEBUFFER_RESIZED = 0x1,
  };
  using SWAPCHAIN_STATE_BITS_T = std::underlying_type_t<SWAPCHAIN_STATE_BITS>;

  // --- Aliases
  template <typename T>
  using NoCapContainer = mini_vec<T>;

  template <typename T>
  using CapContainer = std::vector<T>;
}


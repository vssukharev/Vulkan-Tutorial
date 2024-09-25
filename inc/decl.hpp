  
#include <cstdint>
#include <helpers/mini-vec.hpp>

#pragma once

namespace App {
  // --- Inner structs
  enum class QUEUE_FAMILIES_BITS : uint32_t
  {
    GRAPHICS = 0x01,
    PRESENTATION = 0x02,
  };

  template <typename T>
  using Container = mini_vec<T>;
}


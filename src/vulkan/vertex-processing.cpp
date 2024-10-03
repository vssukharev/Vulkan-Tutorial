
#include <array>
#include <cstddef>
#include <hello-triangle.hpp>
#include <vulkan/vulkan_core.h>


///
void App::SetVertices(
    Vertices& rVertices)
{
  rVertices = {
    { { 0.5, -0.5 }, { 1.0, 0.0, 0.0 }  },
    { { 0.5,  0.5 }, { 0.0, 1.0, 0.0 }  },
    { { -0.5, 0.5 }, { 0.0, 0.0, 1.0 }  },
  };
}


///
VkVertexInputBindingDescription 
  App::GetVertexBindingDescription()
{
  VkVertexInputBindingDescription binding_description {};
  // binding - index of the binding in the array of bindings
  binding_description.binding = 0;
  binding_description.stride = sizeof(Vertex);
  binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return binding_description;
}

///
std::array<VkVertexInputAttributeDescription, 2> 
  App::GetVertexAttributeDescriptions()
{
  std::array<VkVertexInputAttributeDescription, 2> attr_descriptions {};
  attr_descriptions[0].binding = 0;
  attr_descriptions[0].location = 0;
  attr_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attr_descriptions[0].offset = offsetof(Vertex, pos);

  attr_descriptions[1].binding = 0;
  attr_descriptions[1].location = 1;
  attr_descriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attr_descriptions[1].offset = offsetof(Vertex, color);

  return attr_descriptions;
}


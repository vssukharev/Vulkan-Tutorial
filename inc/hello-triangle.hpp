
#pragma once


#include <cstdint>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <helpers/flags.hpp>
#include <decl.hpp>

namespace App {
  // ----------------------- DATA ------------------------

  // ------ Vulkan instances ------

  /// Contains indexes of queue families for selected physical device
  struct Vulkan_QueueFamilies
  {
    uint32_t graphics_family;
    uint32_t present_family;
    flags<QUEUE_FAMILIES_BITS> supported_families;
  };
  
  struct Vulkan_Queues 
  {
    VkQueue graphics_queue;
    VkQueue presentation_queue;
  };

  struct Vulkan_SwapChain 
  {
    std::vector<VkImage> images;
    std::vector<VkImageView> image_views;
    std::vector<VkFramebuffer> framebuffers;
    VkSwapchainKHR handle;
    VkFormat image_format;
    VkExtent2D extent;
  };

  struct Vulkan_Sync
  {
    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;
  };


  // --- Debug struct
  struct Vulkan_Debug 
  {
    VkDebugUtilsMessengerEXT messenger;
  };

  // --- Main struct
  struct Vulkan 
  {
    Vulkan_SwapChain swap_chain;
    Vulkan_Queues queues;
    Vulkan_QueueFamilies queue_families;
    Vulkan_Sync sync;
    VkDevice device;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkSurfaceKHR surface;
    VkInstance instance;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    VkPipeline graphics_pipeline;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;
    GLFWwindow* window;

#ifndef NDEBUG
    Vulkan_Debug dbg;
#endif // !NDEBUG
  };
  
  // ------------------------------
  

  // ------ Root Data struct ------
  struct Data 
  {
    Vulkan vulkan;
  };

  /// Initializes data on heap
  class Data_Class {
    Data* pdata;

    public:
      Data_Class();
      ~Data_Class();
      Data& get();
      operator Data&();
  };
  // ------------------------------

  // -----------------------------------------------------------


  // ---------------------- Code modules -----------------------
  
  // ------ General ------
  void Run();
  void Init(Data&);
  void MainLoop(Data&);
  void Cleanup(Data&);
  // ---------------------


  // ------ Vulkan ------
  void Init(Vulkan&);
  void Cleanup(Vulkan&) noexcept;
  void CreateWindow(Vulkan&);
  void CreateInstance(Vulkan&);
  void CreateSurface(Vulkan&);
  void PickPhysicalDevice(Vulkan&);
  void SetQueueFamilies(Vulkan&);
  void CreateLogicalDevice(Vulkan&);
  void CreateSwapChain(Vulkan&);
  void CreateImageViews(Vulkan&);
  void CreateRenderPass(Vulkan&);
  void CreateGraphicsPipeline(Vulkan&);
  void CreateFramebuffers(Vulkan&);
  void CreateCommandPool(Vulkan&);
  void CreateCommandBuffer(Vulkan&);
  void CreateSyncObjects(Vulkan&);

  void RecordCommandBuffer(Vulkan&, uint32_t image_index);

  void DrawFrame(Vulkan&);
  // --------------------
 

  // ------ Mixed ------
  // -------------------

  // -----------------------------------------------------------
}





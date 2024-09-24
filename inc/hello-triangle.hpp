
#pragma once


#include <cstdint>
#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <helpers/flags.hpp>
#include <decl.hpp>

namespace App {
  // ----------------------- DATA ------------------------

  // ------ Meta information ------
  struct Meta 
  {
    std::filesystem::path binary_dir;
  };

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
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> in_flight_fences;
  };

  struct Vulkan_Frame
  {
    uint32_t current;
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
    std::vector<VkCommandBuffer> command_buffers;
    Vulkan_Frame frame;
    VkDevice device;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkSurfaceKHR surface;
    VkInstance instance;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    VkPipeline graphics_pipeline;
    VkCommandPool command_pool;
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
    Meta meta;
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

  // ------ Meta ------
  void Init(Meta&);
  // ------------------

  // ------ Vulkan ------
  void Init(Vulkan&, Meta&);
  void InitGLFW();
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
  void CreateGraphicsPipeline(Vulkan&, Meta&);
  void CreateFramebuffers(Vulkan&);
  void CreateCommandPool(Vulkan&);
  void CreateCommandBuffers(Vulkan&);
  void CreateSyncObjects(Vulkan&);

  void RecordCommandBuffer(Vulkan&, uint32_t buffer_index, uint32_t image_index);

  void DrawFrame(Vulkan&);
  // --------------------
 

  // ------ Mixed ------
  // -------------------

  // -----------------------------------------------------------
}





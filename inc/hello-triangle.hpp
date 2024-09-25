
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
  
  // --------- Aliases ----------
  template <typename T>
  using Container = std::vector<T>;

  using Window = GLFWwindow*;
  using Images = Container<VkImage>;
  using ImageViews = Container<VkImageView>;
  using Framebuffers = Container<VkFramebuffer>;
  using CommandBuffers = Container<VkCommandBuffer>;
  // ----------------------------


  // ------ Meta information ------
  struct Meta 
  {
    std::filesystem::path binary_dir;
  };
  // ------ Vulkan instances ------

  /// Contains indexes of queue families for selected physical device
  struct QueueFamilies
  {
    uint32_t graphics_family;
    uint32_t present_family;
    flags<QUEUE_FAMILIES_BITS> supported_families;
  };
  
  struct Queues 
  {
    VkQueue graphics_queue;
    VkQueue presentation_queue;
  };

  struct SyncObjects
  {
    Container<VkSemaphore> image_available_semaphores;
    Container<VkSemaphore> render_finished_semaphores;
    Container<VkFence> in_flight_fences;
  };

  struct Frame
  {
  };

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    Container<VkSurfaceFormatKHR> formats;
    Container<VkPresentModeKHR> present_modes;
  };

  // --- Debug struct
  struct Debug 
  {
#ifndef NDEBUG
    VkDebugUtilsMessengerEXT messenger;
#endif // !NDEBUG
  };

  // --- Main struct
  struct Vulkan 
  {
    Images images;
    ImageViews image_views;
    Framebuffers framebuffers;
    Queues queues;
    QueueFamilies queue_families;
    SyncObjects sync;
    Container<VkCommandBuffer> command_buffers;
    uint32_t current_frame;
    VkSwapchainKHR swap_chain;
    VkFormat image_format;
    VkExtent2D extent;
    VkDevice device;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkSurfaceKHR surface;
    VkInstance instance;
    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;
    VkPipeline graphics_pipeline;
    VkCommandPool command_pool;
    GLFWwindow* window;

    // TODO: get rid of debug inside Vulkan class
    Debug dbg;
  };
  
  // ------------------------------
  

  // ------ Root Data struct ------
  struct Data 
  {
    Vulkan vulkan;
    Meta meta;
  };
  // ------------------------------

  // -----------------------------------------------------------


  // ---------------------- Code modules -----------------------
  
  // ------ General ------
  void Init(
      Data&);

  void MainLoop(
      Data&);

  void Cleanup(
      Data&);
  // ---------------------

  // ------ Meta ------
  void Init(
      Meta&);
  // ------------------

  // ------ Vulkan ------
  void Init(
      Vulkan&, 
      Meta&);
  
  void InitGLFW();

  void Cleanup(
      Vulkan&) 
    noexcept;
  
  void CreateWindow(
      Window&);

  void CreateInstance(
      VkInstance&);

  void CreateSurface(
      VkSurfaceKHR&, 
      Window, 
      VkInstance);

  void PickPhysicalDevice(
      VkPhysicalDevice&, 
      VkInstance, 
      VkSurfaceKHR);

  void SetQueueFamilies(
      QueueFamilies&, 
      VkPhysicalDevice, 
      VkSurfaceKHR);

  void CreateLogicalDevice(    
      VkDevice& device, 
      QueueFamilies& qf,
      VkPhysicalDevice physical_device, 
      VkSurfaceKHR surface);

  void SetQueues(
      Queues& queues, 
      const QueueFamilies& qf, 
      VkDevice logical_device);

  void CreateSwapChain(
      VkSwapchainKHR& swap_chain,
      Container<VkImage>& images,
      VkFormat& image_format,
      VkExtent2D& image_extent,
      VkDevice logical_device,
      VkPhysicalDevice physical_device,
      VkSurfaceKHR surface,
      Window window,
      QueueFamilies& qf);

  void CreateImageViews(
      Container<VkImageView>& image_views, 
      const Container<VkImage>& images, 
      VkFormat image_format,
      VkDevice dev);

  void CreateRenderPass(
      VkRenderPass& render_pass,
      VkFormat image_format,
      VkDevice logical_device);
  
  void CreateGraphicsPipeline(
      VkPipeline& graphics_pipeline,
      VkPipelineLayout& pipeline_layout,
      VkDevice logical_device,
      VkRenderPass render_pass,
      const std::filesystem::path& binary_dir);
  
  void CreateFramebuffers(
      Container<VkFramebuffer>& framebuffers,
      const Container<VkImageView>& image_views,
      VkDevice logical_device, 
      VkRenderPass render_pass,
      VkExtent2D framebuffer_extent);

  void CreateCommandPool(
    VkCommandPool& command_pool,
    const QueueFamilies& qf,
    VkDevice logical_device);

  void CreateCommandBuffers(
      Container<VkCommandBuffer>& command_buffers,
      VkDevice logical_device,
      VkCommandPool command_pool);

  void CreateSyncObjects(
      SyncObjects& sync,
      VkDevice logical_device);

  void RecordCommandBuffer(
      VkCommandBuffer& command_buffer,
      VkFramebuffer framebuffer,
      VkExtent2D image_extent,
      VkRenderPass render_pass,
      VkPipeline graphics_pipeline);

  void DrawFrame(
      uint32_t& last_frame,
      Container<VkCommandBuffer>& command_buffers,
      const SyncObjects& sync,
      const Queues& queues,
      const Container<VkFramebuffer>& framebuffers,
      VkDevice logical_device,
      VkSwapchainKHR swap_chain,
      VkExtent2D image_extent,
      VkRenderPass render_pass,
      VkPipeline graphics_pipeline);

  void DrawFrame(Data&);
    // ------ Vulkan ------
  // --- Helper structs
  
  // --- Extensions
  Container<const char*> GetRequiredVulkanExtensions() noexcept;

  // --- Devices
  int RatePhysicalDeviceSuitability(VkPhysicalDevice, QueueFamilies&, SwapChainSupportDetails&) noexcept;
  bool CheckPhysicalDeviceExtensionsSupport(VkPhysicalDevice);
  void QueryQueueFamilies(QueueFamilies&, VkPhysicalDevice, VkSurfaceKHR);
  bool CheckQueueFamiliesSupport(const QueueFamilies&);
  Container<VkDeviceQueueCreateInfo> GetVulkanQueueCreateInfos(QueueFamilies&);
  void QuerySwapChainSupport(SwapChainSupportDetails&, VkPhysicalDevice, VkSurfaceKHR);
  bool CheckSwapChainSupport(const SwapChainSupportDetails&);
  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const Container<VkSurfaceFormatKHR>&);
  int RateSwapSurfaceFormat(VkSurfaceFormatKHR);
  VkPresentModeKHR ChooseSwapPresentMode(const Container<VkPresentModeKHR>&);
  int RateSwapPresentMode(VkPresentModeKHR);
  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR&, GLFWwindow*);
  VkShaderModule CreateShaderModule(VkDevice dev, const Container<char>& code);
  // --------------------

  // --------------------
 

  // ------ Mixed ------
  // -------------------

  // -----------------------------------------------------------
}





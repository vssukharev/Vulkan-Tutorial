
#pragma once

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <helpers/mini-vec.hpp>
#include <decl.hpp>

namespace App {
  // ----------------------- DATA ------------------------
  
  // --------- Aliases ----------
  using Window = GLFWwindow*;
  using Images = CapContainer<VkImage>;
  using ImageViews = CapContainer<VkImageView>;
  using Framebuffers = CapContainer<VkFramebuffer>;
  using CommandBuffers = NoCapContainer<VkCommandBuffer>;
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
    QUEUE_FAMILIES_BITS_T supported_families;
  };
  
  struct Queues 
  {
    VkQueue graphics_queue;
    VkQueue presentation_queue;
  };

  struct SyncObjects
  {
    NoCapContainer<VkSemaphore> image_available_semaphores;
    NoCapContainer<VkSemaphore> render_finished_semaphores;
    NoCapContainer<VkFence>     in_flight_fences;
  };

  struct SwapChainComponents 
  {
    QueueFamilies     queue_families;
    Images            images;
    ImageViews        image_views;
    Framebuffers      framebuffers;
    Window            window;
    VkSwapchainKHR    handle;
    VkFormat          image_format;
    VkExtent2D        extent;
    VkDevice          device;
    VkPhysicalDevice  physical_device = VK_NULL_HANDLE;
    VkSurfaceKHR      surface;
    VkRenderPass      render_pass;
    SWAPCHAIN_STATE_BITS_T state;
  };

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    NoCapContainer<VkSurfaceFormatKHR> formats;
    NoCapContainer<VkPresentModeKHR> present_modes;
  };

  // --- Debug struct
  struct VulkanDebug
  {
    VkDebugUtilsMessengerEXT messenger;
  };

  // --- Main struct
  struct Vulkan 
  {
    SwapChainComponents swap_chain;

    Queues queues;
    SyncObjects sync;
    CommandBuffers command_buffers;

    VkInstance instance;
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;
    VkCommandPool command_pool;

    uint32_t current_frame;

    VulkanDebug dbg;
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
      Data& rData);

  void MainLoop(
      Data& rData);

  void Cleanup(
      Data& rData);
  // ---------------------

  // ------ Meta ------
  void Init(
      Meta& rMeta);
  // ------------------

  // ------ Vulkan ------
  void Init(
      Vulkan&       rVk, 
      VulkanDebug&  rDbg, 
      const Meta&   meta);
  
  void InitGLFW();

  void Cleanup(
      Vulkan&      rVk,
      VulkanDebug& rVkDbg) 
    noexcept;
  
  void CreateWindow(
      Window&         rWindow,
      SWAPCHAIN_STATE_BITS_T& rSwapChainState);

  void CreateInstance(
      VkInstance& rInstance);

  void CreateSurface(
      VkSurfaceKHR& rSurface, 
      Window        window, 
      VkInstance    instance);

  void PickPhysicalDevice(
      VkPhysicalDevice& rPhysicalDevice, 
      VkInstance        instance,
      VkSurfaceKHR      surface);

  void SetQueueFamilies(
      QueueFamilies&    rQueueFamilies,
      VkPhysicalDevice  physicalDevice,
      VkSurfaceKHR      surface);

  void CreateLogicalDevice(    
      VkDevice&        rLogicalDevice, 
      QueueFamilies&   rQueueFamilies,
      VkPhysicalDevice physicalDevice, 
      VkSurfaceKHR     surface);

  void SetQueues(
      Queues&              rQueues, 
      const QueueFamilies& queueFamilies, 
      VkDevice             logicalDevice);

  
  // -------- Swap chain -----------
  // --- Lower-deps functions
  void CreateSwapChain(
      VkSwapchainKHR&       rSwapChain,
      Images&               rImages,
      VkFormat&             rImageFormat,
      VkExtent2D&           rImageExtent,
      VkDevice              logicalDevice,
      VkPhysicalDevice      physicalDevice,
      VkSurfaceKHR          surface,
      Window                window,
      const QueueFamilies&  queueFamilies);
  
  void RecreateSwapChain(
      Framebuffers&        rFramebuffers,
      ImageViews&          rImageViews,
      Images&              rImages,
      VkFormat&            rImageFormat,
      VkExtent2D&          rImageExtent,
      VkSwapchainKHR&      rSwapChain,
      const QueueFamilies& queueFamilies,
      VkDevice             logicalDevice,
      VkPhysicalDevice     physicalDevice,
      VkSurfaceKHR         surface,
      Window               window,
      VkRenderPass         renderPass);

  void CreateImageViews(
      ImageViews&   rImageViews, 
      const Images& images, 
      VkFormat      imageFormat,
      VkDevice      logicalDevice);

  void CreateRenderPass(
      VkRenderPass& rRenderPass,
      VkFormat      imageFormat,
      VkDevice      logicalDevice);
  
  void CreateFramebuffers(
      Framebuffers&     rFramebuffers,
      const ImageViews& imageViews,
      VkDevice          logicalDevice, 
      VkRenderPass      renderPass,
      VkExtent2D        framebufferExtent);

  void CleanupSwapChain(
      Framebuffers&   rFramebuffers,
      ImageViews&     rImageViews,
      VkSwapchainKHR  swapChain,
      VkDevice        logicalDevice);
  
  // --- Swapchain components
  inline void CreateSwapChainComponents(
      SwapChainComponents& rSwapChain)
  {
    CreateSwapChain(
        rSwapChain.handle,
        rSwapChain.images,
        rSwapChain.image_format,
        rSwapChain.extent,
        rSwapChain.device,
        rSwapChain.physical_device,
        rSwapChain.surface,
        rSwapChain.window,
        rSwapChain.queue_families);
    CreateImageViews(
        rSwapChain.image_views, 
        rSwapChain.images, 
        rSwapChain.image_format, 
        rSwapChain.device);
    CreateRenderPass(
        rSwapChain.render_pass, 
        rSwapChain.image_format, 
        rSwapChain.device);
    CreateFramebuffers(
        rSwapChain.framebuffers, 
        rSwapChain.image_views, 
        rSwapChain.device, 
        rSwapChain.render_pass, 
        rSwapChain.extent);
  }

  inline void RecreateSwapChainComponents(
      SwapChainComponents& rSwapChain)
  {
    RecreateSwapChain(
        rSwapChain.framebuffers, 
        rSwapChain.image_views, 
        rSwapChain.images, 
        rSwapChain.image_format, 
        rSwapChain.extent, 
        rSwapChain.handle, 
        rSwapChain.queue_families, 
        rSwapChain.device, 
        rSwapChain.physical_device, 
        rSwapChain.surface, 
        rSwapChain.window, 
        rSwapChain.render_pass);
  }
  // ----------------------------

  void CreateGraphicsPipeline(
      VkPipeline&                  rGraphicsPipeline,
      VkPipelineLayout&            rPipelineLayout,
      VkDevice                     logicalDevice,
      VkRenderPass                 renderPass,
      const std::filesystem::path& mainBinaryDir);

  void CreateCommandPool(
      VkCommandPool&        rCommandPool,
      const QueueFamilies&  queueFamilies,
      VkDevice              logicalDevice);

  void CreateCommandBuffers(
      CommandBuffers& rCommandBuffers,
      VkDevice        logicalDevice,
      VkCommandPool   commandPool);

  void CreateSyncObjects(
      SyncObjects&  rSyncObjects,
      VkDevice      logicalDevice);

  void RecordCommandBuffer(
      VkCommandBuffer&  rCommandBuffer,
      VkFramebuffer     framebuffer,
      VkExtent2D        imageExtent,
      VkRenderPass      renderPass,
      VkPipeline        graphicsPipeline);

  void DrawFrame(
      uint32_t&           rLastFrame,
      CommandBuffers&     rCommandBuffers,
      SwapChainComponents& rSwapChain,
      const SyncObjects&  syncObjects,
      const Queues&       queues,
      VkPipeline          graphicsPipeline);

  void DrawFrame(Data& rData);
    // ------ Vulkan ------
  // --- Helper structs
  
  // --- Extensions
  std::vector<const char*> GetRequiredVulkanExtensions() noexcept;

  // --- Devices
  int RatePhysicalDeviceSuitability(
      VkPhysicalDevice, 
      QueueFamilies&, 
      SwapChainSupportDetails&) noexcept;

  bool CheckPhysicalDeviceExtensionsSupport(
      VkPhysicalDevice);

  void QueryQueueFamilies(
      QueueFamilies&, 
      VkPhysicalDevice, 
      VkSurfaceKHR);

  bool CheckQueueFamiliesSupport(
      const QueueFamilies&);

  NoCapContainer<VkDeviceQueueCreateInfo> GetVulkanQueueCreateInfos(
      QueueFamilies&);

  void QuerySwapChainSupport(
      SwapChainSupportDetails&, 
      VkPhysicalDevice, 
      VkSurfaceKHR);

  bool CheckSwapChainSupport(
      const SwapChainSupportDetails&);

  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
      const NoCapContainer<VkSurfaceFormatKHR>&);

  int RateSwapSurfaceFormat(
      VkSurfaceFormatKHR);

  VkPresentModeKHR ChooseSwapPresentMode(
      const NoCapContainer<VkPresentModeKHR>&);

  int RateSwapPresentMode(
      VkPresentModeKHR);

  VkExtent2D ChooseSwapExtent(
      const VkSurfaceCapabilitiesKHR&, 
      Window);

  VkShaderModule CreateShaderModule(
      VkDevice           logicalDevice, 
      const std::string& code);

  // --- Callbacks
  void FramebufferResizeCallback(
      GLFWwindow* window, 
      int         width,
      int         height);

  // --------------------

  // --------------------
 

  // ------ Mixed ------
  // -------------------

  // -----------------------------------------------------------
}






#pragma once

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include <array>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <helpers/mini-vec.hpp>
#include <decl.hpp>

namespace App {
  // ----------------------- DATA ------------------------
  
  // --------- Helper structs ----------
  struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
  };

  struct SwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;
    NoCapContainer<VkSurfaceFormatKHR> formats;
    NoCapContainer<VkPresentModeKHR> present_modes;
  };
  // -----------------------------------

  // --------- Aliases ----------
  using Window = GLFWwindow*;
  using Images = CapContainer<VkImage>;
  using ImageViews = CapContainer<VkImageView>;
  using Framebuffers = CapContainer<VkFramebuffer>;
  using Vertices = NoCapContainer<Vertex>;
  // ----------------------------


  // ------ Meta information ------
  struct Meta 
  {
    std::filesystem::path binary_dir;
  };
  // ------------------------------ 

  // ------ Vulkan instances ------
  /// Contains indexes of queue families for selected physical device
  struct QueueFamilies
  {
    uint32_t graphics;
    uint32_t present;
    uint32_t transfer;
    QUEUE_FAMILIES_BITS_T supported_families;
  };
  
  struct Queues 
  {
    VkQueue graphics;
    VkQueue presentation;
    VkQueue transfer;
  };

  struct CommandPools 
  {
    VkCommandPool graphics;
    VkCommandPool transfer;
  };

  struct CommandBuffers
  {
    NoCapContainer<VkCommandBuffer> graphics;
    NoCapContainer<VkCommandBuffer> transfer;
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
    CommandPools command_pools;

    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_mem;
    Vertices vertices;

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

  void CreateCommandPools(
      CommandPools&         rCommandPool,
      const QueueFamilies&  queueFamilies,
      VkDevice              logicalDevice);

  void SetVertices(
      Vertices& rVertices);

  void CreateVertexBuffer(
      VkBuffer&             rBuffer,
      VkDeviceMemory&       rBufferMemory,
      const Vertices&       vertices,
      const QueueFamilies&  queueFamilies,
      VkDevice              logicalDevice,
      VkPhysicalDevice      physicalDevice);

  void CreateCommandBuffers(
      CommandBuffers&     rCommandBuffers,
      VkDevice            logicalDevice,
      const CommandPools& commandPools);

  void CreateSyncObjects(
      SyncObjects&  rSyncObjects,
      VkDevice      logicalDevice);

  void RecordCommandBuffer(
      VkCommandBuffer&  rCommandBuffer,
      const Vertices&   vertices,
      VkFramebuffer     framebuffer,
      VkExtent2D        imageExtent,
      VkRenderPass      renderPass,
      VkPipeline        graphicsPipeline,
      VkBuffer          vertexBuffer);

  void DrawFrame(
      uint32_t&             rLastFrame,
      CommandBuffers&       rCommandBuffers,
      SwapChainComponents&  rSwapChain,
      const SyncObjects&    sync,
      const Queues&         queues,
      const Vertices&       vertices,
      VkPipeline            graphicsPipeline,
      VkBuffer              vertexBuffer);

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

  VkVertexInputBindingDescription GetVertexBindingDescription();
  
  std::array<VkVertexInputAttributeDescription, 2> GetVertexAttributeDescriptions();

  uint32_t FindMemoryType(
      VkPhysicalDevice      physicalDevice,
      uint32_t              typeFilter,
      VkMemoryPropertyFlags properties);

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






#pragma once


#include <cstdint>
#include <filesystem>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <helpers/flags.hpp>
#include <helpers/mini-vec.hpp>
#include <decl.hpp>

namespace App {
  // ----------------------- DATA ------------------------
  
  // --------- Aliases ----------
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
  struct VulkanDebug
  {
    VkDebugUtilsMessengerEXT messenger;
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
    CommandBuffers command_buffers;
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
      Window& rWindow);

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

  void CreateImageViews(
      ImageViews&   rImageViews, 
      const Images& images, 
      VkFormat      imageFormat,
      VkDevice      logicalDevice);

  void CreateRenderPass(
      VkRenderPass& rRenderPass,
      VkFormat      imageFormat,
      VkDevice      logicalDevice);
  
  void CreateGraphicsPipeline(
      VkPipeline&                  rGraphicsPipeline,
      VkPipelineLayout&            rPipelineLayout,
      VkDevice                     logicalDevice,
      VkRenderPass                 renderPass,
      const std::filesystem::path& mainBinaryDir);
  
  void CreateFramebuffers(
      Framebuffers&     rFramebuffers,
      const ImageViews& imageViews,
      VkDevice          logicalDevice, 
      VkRenderPass      renderPass,
      VkExtent2D        framebufferExtent);

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
      const SyncObjects&  syncObjects,
      const Queues&       queues,
      const Framebuffers& framebuffers,
      VkDevice            logicalDevice,
      VkSwapchainKHR      swapchain,
      VkExtent2D          imageExtent,
      VkRenderPass        renderPass,
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
  VkShaderModule CreateShaderModule(VkDevice dev, const std::string& code);
  // --------------------

  // --------------------
 

  // ------ Mixed ------
  // -------------------

  // -----------------------------------------------------------
}





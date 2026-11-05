#include <EGP/GraphicsApi/vulkan_impl.hpp>

#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace EGP {

namespace {

constexpr const char* kValidationLayer = "VK_LAYER_KHRONOS_validation";

constexpr const char* kDeviceExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

constexpr std::uint32_t MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
constexpr bool kEnableValidation = false;
#else
constexpr bool kEnableValidation = true;
#endif

auto validationLayersSupported() -> bool {
  for (const auto& layer : vk::enumerateInstanceLayerProperties()) {
    if (std::string(layer.layerName.data()) == kValidationLayer) {
      return true;
    }
  }
  return false;
}

auto requiredInstanceExtensions() -> std::vector<const char*> {
  std::uint32_t count = 0;
  const char** names = glfwGetRequiredInstanceExtensions(&count);
  if (names == nullptr) {
    throw std::runtime_error("GLFW does not support Vulkan");
  }
  return std::vector<const char*>(names, names + count);
}

auto debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                   VkDebugUtilsMessageTypeFlagsEXT,
                   const VkDebugUtilsMessengerCallbackDataEXT* data,
                   void*) -> VKAPI_ATTR VkBool32 {
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    if (data != nullptr && data->pMessage != nullptr) {
      std::cerr << "[vulkan] " << data->pMessage << '\n';
    }
  }
  return VK_FALSE;
}

}  // namespace

VulkanAPI::VulkanAPI(void* nativeWindowHandle)
    : nativeWindowHandle_(nativeWindowHandle) {}

VulkanAPI::~VulkanAPI() = default;

auto VulkanAPI::init(void* nativeWindowHandle) -> void {
  nativeWindowHandle_ = nativeWindowHandle;
  createInstance();
  createDebugMessenger();
  createSurface();
  pickPhysicalDevice();
  createDevice();
  createSwapchain();
  createImageViews();
  createRenderPass();
  createFramebuffers();
  createCommandPool();
  createCommandBuffers();
  createSyncObjects();
}

auto VulkanAPI::clear(const glm::vec4& color, float depth) -> void {
  clearColor_ = color;
  clearDepth_ = depth;
}

auto VulkanAPI::drawFrame() -> void {
  auto& fence = inFlightFences_[currentFrame_];
  const auto waitResult = device_.waitForFences(fence, VK_TRUE, UINT64_MAX);
  if (waitResult != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to wait for in-flight fence");
  }

  std::uint32_t imageIndex = 0;
  const auto acquireResult = device_.acquireNextImageKHR(
      swapchain_, UINT64_MAX, imageAvailableSemaphores_[currentFrame_], nullptr,
      &imageIndex);
  if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
    return;
  }
  if (acquireResult != vk::Result::eSuccess &&
      acquireResult != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error("Failed to acquire swapchain image");
  }

  device_.resetFences(fence);
  recordCommandBuffer(commandBuffers_[currentFrame_], imageIndex);

  const vk::PipelineStageFlags waitStage =
      vk::PipelineStageFlagBits::eColorAttachmentOutput;
  vk::SubmitInfo submitInfo;
  submitInfo.setWaitSemaphores(imageAvailableSemaphores_[currentFrame_]);
  submitInfo.setWaitDstStageMask(waitStage);
  submitInfo.setCommandBuffers(commandBuffers_[currentFrame_]);
  submitInfo.setSignalSemaphores(renderFinishedSemaphores_[imageIndex]);
  graphicsQueue_.submit(submitInfo, fence);

  vk::PresentInfoKHR presentInfo;
  presentInfo.setWaitSemaphores(renderFinishedSemaphores_[imageIndex]);
  presentInfo.setSwapchains(swapchain_);
  presentInfo.setImageIndices(imageIndex);
  const auto presentResult = presentQueue_.presentKHR(presentInfo);
  if (presentResult == vk::Result::eErrorOutOfDateKHR ||
      presentResult == vk::Result::eSuboptimalKHR) {
  } else if (presentResult != vk::Result::eSuccess) {
    throw std::runtime_error("Failed to present swapchain image");
  }

  currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES_IN_FLIGHT;
}

auto VulkanAPI::recordCommandBuffer(vk::CommandBuffer commandBuffer,
                                    std::uint32_t imageIndex) -> void {
  vk::CommandBufferBeginInfo beginInfo;
  commandBuffer.begin(beginInfo);

  vk::ClearValue clearValue;
  vk::ClearColorValue color;
  color.setFloat32({clearColor_.r, clearColor_.g, clearColor_.b, clearColor_.a});
  clearValue.setColor(color);

  vk::RenderPassBeginInfo renderPassInfo;
  renderPassInfo.setRenderPass(renderPass_);
  renderPassInfo.setFramebuffer(framebuffers_[imageIndex]);
  renderPassInfo.setRenderArea(vk::Rect2D{{0, 0}, swapchainExtent_});
  renderPassInfo.setClearValues(clearValue);

  commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
  commandBuffer.endRenderPass();
  commandBuffer.end();
}

auto VulkanAPI::createInstance() -> void {
  vk::ApplicationInfo appInfo;
  appInfo.setPApplicationName("EGP");
  appInfo.setApplicationVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setPEngineName("EGP");
  appInfo.setEngineVersion(VK_MAKE_VERSION(0, 0, 1));
  appInfo.setApiVersion(VK_API_VERSION_1_3);

  auto layers = std::vector<const char*>{};
  if (kEnableValidation) {
    if (!validationLayersSupported()) {
      throw std::runtime_error("Validation layer not available");
    }
    layers.push_back(kValidationLayer);
  }

  auto extensions = requiredInstanceExtensions();
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

  vk::InstanceCreateInfo createInfo;
  createInfo.setPApplicationInfo(&appInfo);
  createInfo.setPEnabledLayerNames(layers);
  createInfo.setPEnabledExtensionNames(extensions);

  instance_ = vk::createInstance(createInfo);
}

auto VulkanAPI::createDebugMessenger() -> void {
  if (!kEnableValidation) {
    return;
  }
  vk::DebugUtilsMessengerCreateInfoEXT createInfo;
  createInfo.setMessageSeverity(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  createInfo.setMessageType(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
  createInfo.setPfnUserCallback(&debugCallback);
  vk::detail::DispatchLoaderDynamic dispatch(instance_, vkGetInstanceProcAddr);
  debugMessenger_ =
      instance_.createDebugUtilsMessengerEXT(createInfo, nullptr, dispatch);
}

auto VulkanAPI::createSurface() -> void {
  if (nativeWindowHandle_ == nullptr) {
    throw std::runtime_error("Native window handle is null");
  }
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  const auto result = glfwCreateWindowSurface(
      instance_, static_cast<GLFWwindow*>(nativeWindowHandle_), nullptr,
      &surface);
  if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }
  surface_ = vk::SurfaceKHR(surface);
}

auto VulkanAPI::pickPhysicalDevice() -> void {
  for (const auto& candidate : instance_.enumeratePhysicalDevices()) {
    std::uint32_t graphicsFamily = UINT32_MAX;
    std::uint32_t presentFamily = UINT32_MAX;
    const auto families = candidate.getQueueFamilyProperties();
    for (std::uint32_t i = 0; i < families.size(); ++i) {
      if ((families[i].queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags{}) {
        graphicsFamily = i;
      }
      if (candidate.getSurfaceSupportKHR(i, surface_) == VK_TRUE) {
        presentFamily = i;
      }
    }
    if (graphicsFamily == UINT32_MAX || presentFamily == UINT32_MAX) {
      continue;
    }
    std::uint32_t formatCount = 0;
    std::uint32_t presentModeCount = 0;
    candidate.getSurfaceFormatsKHR(surface_, &formatCount, nullptr);
    candidate.getSurfacePresentModesKHR(surface_, &presentModeCount, nullptr);
    if (formatCount == 0 || presentModeCount == 0) {
      continue;
    }
    physicalDevice_ = candidate;
    graphicsQueueFamily_ = graphicsFamily;
    presentQueueFamily_ = presentFamily;
    return;
  }
  throw std::runtime_error("No suitable Vulkan physical device found");
}

auto VulkanAPI::createDevice() -> void {
  const std::set<std::uint32_t> families{graphicsQueueFamily_, presentQueueFamily_};
  const float priority = 1.0F;
  std::vector<vk::DeviceQueueCreateInfo> queueInfos;
  for (const auto family : families) {
    vk::DeviceQueueCreateInfo info;
    info.setQueueFamilyIndex(family);
    info.setQueueCount(1);
    info.setPQueuePriorities(&priority);
    queueInfos.push_back(info);
  }

  const auto features = physicalDevice_.getFeatures();
  vk::PhysicalDeviceFeatures enabled;
  enabled.setSamplerAnisotropy(features.samplerAnisotropy);

  vk::DeviceCreateInfo createInfo;
  createInfo.setQueueCreateInfos(queueInfos);
  createInfo.setPEnabledFeatures(&enabled);
  createInfo.setPEnabledExtensionNames(kDeviceExtensions);

  device_ = physicalDevice_.createDevice(createInfo);
  graphicsQueue_ = device_.getQueue(graphicsQueueFamily_, 0);
  presentQueue_ = device_.getQueue(presentQueueFamily_, 0);
}

auto VulkanAPI::createSwapchain() -> void {
  const auto capabilities = physicalDevice_.getSurfaceCapabilitiesKHR(surface_);

  auto format = vk::Format::eB8G8R8A8Srgb;
  auto colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
  const auto availableFormats = physicalDevice_.getSurfaceFormatsKHR(surface_);
  bool formatFound = false;
  for (const auto& candidate : availableFormats) {
    if (candidate.format == format && candidate.colorSpace == colorSpace) {
      formatFound = true;
      break;
    }
  }
  if (!formatFound) {
    format = availableFormats.front().format;
    colorSpace = availableFormats.front().colorSpace;
  }

  auto presentMode = vk::PresentModeKHR::eFifo;
  for (const auto& candidate : physicalDevice_.getSurfacePresentModesKHR(surface_)) {
    if (candidate == vk::PresentModeKHR::eMailbox) {
      presentMode = candidate;
      break;
    }
  }

  vk::Extent2D extent = capabilities.currentExtent;
  if (extent.width == UINT32_MAX) {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(static_cast<GLFWwindow*>(nativeWindowHandle_), &width,
                           &height);
    extent.width = std::clamp(static_cast<std::uint32_t>(width),
                              capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height = std::clamp(static_cast<std::uint32_t>(height),
                               capabilities.minImageExtent.height,
                               capabilities.maxImageExtent.height);
  }

  std::uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  vk::SwapchainCreateInfoKHR createInfo;
  createInfo.setSurface(surface_);
  createInfo.setMinImageCount(imageCount);
  createInfo.setImageFormat(format);
  createInfo.setImageColorSpace(colorSpace);
  createInfo.setImageExtent(extent);
  createInfo.setImageArrayLayers(1);
  createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
  createInfo.setPreTransform(capabilities.currentTransform);
  createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
  createInfo.setPresentMode(presentMode);
  createInfo.setClipped(VK_TRUE);
  createInfo.setImageSharingMode(vk::SharingMode::eExclusive);

  const std::array<std::uint32_t, 2> familyIndices{graphicsQueueFamily_,
                                                   presentQueueFamily_};
  if (graphicsQueueFamily_ != presentQueueFamily_) {
    createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
    createInfo.setQueueFamilyIndices(familyIndices);
  }

  swapchain_ = device_.createSwapchainKHR(createInfo);
  swapchainFormat_ = format;
  swapchainExtent_ = extent;
  swapchainImages_ = device_.getSwapchainImagesKHR(swapchain_);
}

auto VulkanAPI::createImageViews() -> void {
  swapchainImageViews_.clear();
  swapchainImageViews_.reserve(swapchainImages_.size());
  for (const auto& image : swapchainImages_) {
    vk::ImageViewCreateInfo createInfo;
    createInfo.setImage(image);
    createInfo.setViewType(vk::ImageViewType::e2D);
    createInfo.setFormat(swapchainFormat_);
    vk::ComponentMapping components;
    components.setR(vk::ComponentSwizzle::eIdentity);
    components.setG(vk::ComponentSwizzle::eIdentity);
    components.setB(vk::ComponentSwizzle::eIdentity);
    components.setA(vk::ComponentSwizzle::eIdentity);
    createInfo.setComponents(components);
    vk::ImageSubresourceRange range;
    range.setAspectMask(vk::ImageAspectFlagBits::eColor);
    range.setBaseMipLevel(0);
    range.setLevelCount(1);
    range.setBaseArrayLayer(0);
    range.setLayerCount(1);
    createInfo.setSubresourceRange(range);
    swapchainImageViews_.push_back(device_.createImageView(createInfo));
  }
}

auto VulkanAPI::createRenderPass() -> void {
  vk::AttachmentDescription colorAttachment;
  colorAttachment.setFormat(swapchainFormat_);
  colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
  colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
  colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
  colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
  colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
  colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
  colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference colorRef;
  colorRef.setAttachment(0);
  colorRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  vk::SubpassDescription subpass;
  subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
  subpass.setColorAttachments(colorRef);

  vk::SubpassDependency dependency;
  dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL);
  dependency.setDstSubpass(0);
  dependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setSrcAccessMask(vk::AccessFlags{});
  dependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
  dependency.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

  vk::RenderPassCreateInfo createInfo;
  createInfo.setAttachments(colorAttachment);
  createInfo.setSubpasses(subpass);
  createInfo.setDependencies(dependency);

  renderPass_ = device_.createRenderPass(createInfo);
}

auto VulkanAPI::createFramebuffers() -> void {
  framebuffers_.clear();
  framebuffers_.reserve(swapchainImageViews_.size());
  for (const auto& view : swapchainImageViews_) {
    vk::FramebufferCreateInfo createInfo;
    createInfo.setRenderPass(renderPass_);
    createInfo.setAttachments(view);
    createInfo.setWidth(swapchainExtent_.width);
    createInfo.setHeight(swapchainExtent_.height);
    createInfo.setLayers(1);
    framebuffers_.push_back(device_.createFramebuffer(createInfo));
  }
}

auto VulkanAPI::createCommandPool() -> void {
  vk::CommandPoolCreateInfo createInfo;
  createInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  createInfo.setQueueFamilyIndex(graphicsQueueFamily_);
  commandPool_ = device_.createCommandPool(createInfo);
}

auto VulkanAPI::createCommandBuffers() -> void {
  commandBuffers_.resize(framebuffers_.size());
  vk::CommandBufferAllocateInfo allocateInfo;
  allocateInfo.setCommandPool(commandPool_);
  allocateInfo.setLevel(vk::CommandBufferLevel::ePrimary);
  allocateInfo.setCommandBufferCount(
      static_cast<std::uint32_t>(commandBuffers_.size()));
  commandBuffers_ = device_.allocateCommandBuffers(allocateInfo);
}

auto VulkanAPI::createSyncObjects() -> void {
  const auto frameCount = static_cast<std::size_t>(MAX_FRAMES_IN_FLIGHT);
  vk::SemaphoreCreateInfo semaphoreInfo;
  vk::FenceCreateInfo fenceInfo;
  fenceInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

  imageAvailableSemaphores_.resize(frameCount);
  inFlightFences_.resize(frameCount);
  for (std::size_t i = 0; i < frameCount; ++i) {
    imageAvailableSemaphores_[i] = device_.createSemaphore(semaphoreInfo);
    inFlightFences_[i] = device_.createFence(fenceInfo);
  }

  renderFinishedSemaphores_.resize(swapchainImages_.size());
  for (auto& semaphore : renderFinishedSemaphores_) {
    semaphore = device_.createSemaphore(semaphoreInfo);
  }
}

}  // namespace EGP

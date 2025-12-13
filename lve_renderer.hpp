#pragma once

#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {
class LveRenderer {
 public:

  LveRenderer(LveWindow &window, LveDevice &device);
  ~LveRenderer();

  LveRenderer(const LveRenderer &) = delete;
  LveRenderer &operator=(const LveRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {return lveSwapChain->getRenderPass();}
  bool isFrameInProgress() const {return isFrameStarted;}

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted && "Cannot get commandbuffer when frame not in progress");
    return commandBuffers[currentImageIndex];
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commmandBuffer);


 private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  LveWindow& lveWindow;
  LveDevice& lveDevice;
  std::unique_ptr<LveSwapChain> lveSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  bool isFrameStarted;
};
}  // namespace lve

#pragma once

#include "lve_device.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve
{
  class LveRenderer
  {
  public:
    LveRenderer(LveWindow &window, LveDevice &device);
    ~LveRenderer();

    LveRenderer(const LveRenderer &) = delete;
    LveRenderer &operator=(const LveRenderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const { return lveSwapChain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
      assert(isFrameStarted && "Cannot get commandbuffer when frame not in progress");
      return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const
    {
      assert(isFrameStarted && "Cannot get frame index when fraem is not in progress");
      return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commmandBuffer);

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    LveWindow &lveWindow;
    LveDevice &lveDevice;
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::vector<VkCommandBuffer> commandBuffers;
    // track frame/image indices and state; initialize to safe defaults to avoid UB
    uint32_t currentImageIndex = 0;
    int currentFrameIndex = 0;
    bool isFrameStarted = false;
  };
} // namespace lve

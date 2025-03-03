#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "../wrapper/LogicalDeviceWrapper.h"
#include "SwapChain.h"

class SwapChain;

class Renderer {

private:
    std::vector<VkCommandBuffer> commandBuffers;
    VkCommandPool commandPool;

    std::unique_ptr<SwapChain> swapChain;

    void createCommandBuffers();
    void createCommandPool();

    void cleanupCommandPool() {
        vkDestroyCommandPool(LogicalDeviceWrapper::getVkDevice(), commandPool, nullptr);
    }

public:
    Renderer(Window* window) : swapChain(std::make_unique<SwapChain>(window)) {
        createCommandPool();
        createCommandBuffers();
    }

    ~Renderer() {
        cleanupCommandPool();
    }

    std::vector<VkCommandBuffer>& getCommandBuffers() {
        return commandBuffers;
    };

    VkCommandPool& getCommandPool() {
        return commandPool;
    }

    std::unique_ptr<SwapChain>& getSwapChain() {
        return swapChain;
    }
}; 
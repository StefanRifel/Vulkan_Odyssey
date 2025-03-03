#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <array>

#include "../wrapper/LogicalDeviceWrapper.h"

class RenderPass {

private:
    VkRenderPass renderPass;
    VkDevice device;

public:
    RenderPass() noexcept : renderPass(VK_NULL_HANDLE), device(VK_NULL_HANDLE) {}

    explicit RenderPass(VkDevice logicalDevice, VkFormat swapChainImageFormat, VkFormat depthFormat)
        : device(logicalDevice), renderPass(VK_NULL_HANDLE) {
        createRenderPass(swapChainImageFormat, depthFormat);
    }

    ~RenderPass() {
        cleanup();
    }

    void cleanup() {
        if (renderPass != VK_NULL_HANDLE) {
            vkDestroyRenderPass(device, renderPass, nullptr);
            renderPass = VK_NULL_HANDLE;
        }
    }

    VkRenderPass& getRenderPass() {
        return renderPass;
    };

    void createRenderPass(VkFormat& swapChainImageFormat, VkFormat depthFormat);
};
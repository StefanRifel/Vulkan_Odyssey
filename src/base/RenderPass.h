#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <array>

#include "../wrapper/LogicalDeviceWrapper.h"

class RenderPass {

private:
    VkDevice device;
    VkRenderPass renderPass;

public:

    RenderPass(VkFormat swapChainImageFormat, VkFormat depthFormat)
        : device(VK_NULL_HANDLE), renderPass(VK_NULL_HANDLE) {
        createRenderPass(swapChainImageFormat, depthFormat);
    }

    ~RenderPass() {
        cleanup();
    }

    void cleanup() {
        vkDestroyRenderPass(LogicalDeviceWrapper::getVkDevice(), renderPass, nullptr);
    }

    VkRenderPass& getRenderPass() {
        return renderPass;
    };

    void createRenderPass(VkFormat& swapChainImageFormat, VkFormat depthFormat);
};
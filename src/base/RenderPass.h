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
    RenderPass() noexcept : device(VK_NULL_HANDLE), renderPass(VK_NULL_HANDLE) {}

    RenderPass(VkFormat swapChainImageFormat, VkFormat depthFormat)
        : device(VK_NULL_HANDLE), renderPass(VK_NULL_HANDLE) {
        createRenderPass(swapChainImageFormat, depthFormat);
    }

    ~RenderPass() {
        std::cout << "Destroying RenderPass" << std::endl;
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
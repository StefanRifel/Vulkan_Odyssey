#ifndef RENDERPASS_H
#define RENDERPASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <stdexcept>

#include "../wrapper/LogicalDeviceWrapper.h"

class RenderPass {

private:
    VkRenderPass renderPass;

public:
    RenderPass() {
        renderPass = VK_NULL_HANDLE;
    };

    VkRenderPass& getRenderPass() {
        return renderPass;
    };

    void createRenderPass(VkFormat& swapChainImageFormat, VkFormat depthFormat);
};

#endif
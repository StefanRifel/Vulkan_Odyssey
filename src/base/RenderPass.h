#ifndef RENDERPASS_H
#define RENDERPASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../types/Vertex.h"

#include "../loader/FileLoader.h"

#include "../base/DescriptorPool.h"
#include "../base/SwapChain.h"

#include <array>

class RenderPass {

private:
    VkRenderPass renderPass;

public:
    RenderPass() {
        createRenderPass();
    }
    
    VkRenderPass& getRenderPass() {
        return renderPass;
    };

    void createRenderPass();
};

#endif
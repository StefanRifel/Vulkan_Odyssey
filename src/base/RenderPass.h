#ifndef RENDERPASS_H
#define RENDERPASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../Shader.h"

#include "../types/Vertex.h"

#include "../loader/FileLoader.h"

#include "../base/Descriptor.h"
#include "../base/SwapChain.h"

#include <array>

class RenderPass {

private:
    static VkRenderPass renderPass;
    
    static VkPipelineLayout pipelineLayout;
    static VkPipeline graphicsPipeline;

public:
    static VkRenderPass& getRenderPass();
    static VkPipelineLayout& getPipelineLayout();
    static VkPipeline& getGraphicsPipeline();

    static void createRenderPass();
    static void createGraphicsPipeline();
};

#endif
#ifndef RENDERPASS_H
#define RENDERPASS_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../Shader.h"

#include "../types/Vertex.h"

#include "../loader/FileLoader.h"

#include "../base/DescriptorPool.h"
#include "../base/SwapChain.h"

#include <array>

struct GraphicsPipeline {
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

class RenderPass {

private:
    static VkRenderPass renderPass;

public:
    static VkRenderPass& getRenderPass();

    static void createRenderPass();
    static GraphicsPipeline createGraphicsPipeline(std::string vertShaderPath, std::string fragShaderPath);
};

#endif
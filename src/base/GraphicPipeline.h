#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"
#include "../loader/FileLoader.h"
#include "../wrapper/LogicalDeviceWrapper.h"
#include "../types/TransformPushConstantData.h"
#include "SwapChain.h"
#include "DescriptorPool.h"

#include <string>
#include <vector>

// Konfiguration für eine Grafikpipeline
struct GraphicPipelineInfo {
    VkFrontFace frontFace;         // Definiert die Ausrichtung der Vorderseite von Polygonen (im Uhrzeigersinn oder gegen den Uhrzeigersinn)
    VkCullModeFlags cullMode;      // Definiert die Culling-Methode (z. B. Culling von Rückseiten)
    VkCompareOp depthCompare;      // Definiert die Vergleichsfunktion, die für den Tiefentest verwendet wird
};

// Klasse zur Kapselung der Vulkan-Grafikpipeline-Funktionalität
class GraphicPipeline {
private:
    VkPipeline pipeline;
    VkPipelineLayout layout;

    GraphicPipelineInfo graphicPipelineInfo;

    void createGraphicsPipeline(
        const std::string& vertShaderPath, 
        const std::string& fragShaderPath, 
        const GraphicPipelineInfo& graphicPipelineInfo,
        RenderPass* renderPass,
        VkDescriptorSetLayout& descriptorSetLayout);

    VkShaderModule createShaderModule(const std::vector<char>& code);

public:
    explicit GraphicPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const GraphicPipelineInfo& graphicPipelineInfo,
        RenderPass* renderPass, VkDescriptorSetLayout descriptorSetLayout) : graphicPipelineInfo {graphicPipelineInfo} {
        createGraphicsPipeline(vertShaderPath, fragShaderPath, graphicPipelineInfo, renderPass, descriptorSetLayout);
    };

    ~GraphicPipeline() {
        std::cout << "Destroying GraphicPipeline" << std::endl;
        vkDestroyPipeline(LogicalDeviceWrapper::getVkDevice(), pipeline, nullptr);
        vkDestroyPipelineLayout(LogicalDeviceWrapper::getVkDevice(), layout, nullptr);
    };
    
    void bind(VkCommandBuffer commandBuffer, SwapChain* swapChain);

    static GraphicPipelineInfo getDefaultGraphicPipelineInfo();

    VkPipeline& getPipeline() { return pipeline; }
    VkPipelineLayout& getLayout() { return layout; }
};
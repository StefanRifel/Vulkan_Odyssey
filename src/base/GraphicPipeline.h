#ifndef GRAPHICPIPELINE_H
#define GRAPHICPIPELINE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "RenderPass.h"
#include "../types/Vertex.h"
#include "../loader/FileLoader.h"
#include "../wrapper/LogicalDeviceWrapper.h"

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

    void createGraphicsPipeline(
        const std::string& vertShaderPath, 
        const std::string& fragShaderPath, 
        const GraphicPipelineInfo& graphicPipelineInfo);

    VkShaderModule createShaderModule(const std::vector<char>& code);

public:
    GraphicPipeline(
        const std::string& vertShaderPath, 
        const std::string& fragShaderPath, 
        const GraphicPipelineInfo& graphicPipelineInfo);

    ~GraphicPipeline();
    
    void bind(VkCommandBuffer commandBuffer);

    static GraphicPipelineInfo getDefaultGraphicPipelineInfo();

    VkPipeline& getPipeline() { return pipeline; }
    VkPipelineLayout& getLayout() { return layout; }
};

#endif
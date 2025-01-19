#include "GraphicPipeline.h"

GraphicPipeline::GraphicPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const GraphicPipelineInfo& graphicPipelineInfo) {
    createGraphicsPipeline(vertShaderPath, fragShaderPath, graphicPipelineInfo);
}

GraphicPipeline::~GraphicPipeline() {
    Logger::log("GRAPHICSPIPELINE::DESTRUCTOR::Destroying graphics pipeline and layout");
    vkDestroyPipeline(LogicalDeviceWrapper::getVkDevice(), pipeline, nullptr);
    vkDestroyPipelineLayout(LogicalDeviceWrapper::getVkDevice(), layout, nullptr);
}

GraphicPipelineInfo GraphicPipeline::getDefaultGraphicPipelineInfo() {
    return {
        VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_CULL_MODE_BACK_BIT,
        VK_COMPARE_OP_LESS
    };
}

void GraphicPipeline::createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const GraphicPipelineInfo& graphicPipelineInfo) {
    // Lade die Shader-Module
    VkShaderModule vertShaderModule = createShaderModule(readFile(vertShaderPath));
    VkShaderModule fragShaderModule = createShaderModule(readFile(fragShaderPath));
    
    // Konfiguration der Shader-Stufen: Definiert die Shader-Module, die in der Grafikpipeline verwendet werden
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};


    // Konfiguration der Vertex-Input-Stufe: Definiert die Formate der Vertex-Daten
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


    // Configure the fixed-function stages
    // Konfiguration der Input-Assembly-Stufe: Bestimmt, wie die Vertex-Daten interpretiert werden
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};                                 
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;                           // Topologie legt fest, wie die Primitive (z. B. Dreiecke, Linien) aus den Vertex-Daten erzeugt werden.
    inputAssembly.primitiveRestartEnable = VK_FALSE;                                        // Aktiviert oder deaktiviert das Neustarten von Primitiven bei einem bestimmten Indexwert.

    // Konfiguration der Viewport-Stufe: Definiert den Viewport und das Scissor-Rechteck
    VkPipelineViewportStateCreateInfo viewportState{};                                      
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;                                                        // Anzahl der Viewports
    viewportState.scissorCount = 1;                                                         // Anzahl der Scissor-Rechtecke

    // Konfiguration der Rasterizer-Stufe: Wandelt Geometrie in Fragmente um
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;          
    rasterizer.depthClampEnable = VK_FALSE;                                                 //Clamping der Tiefe von Fragmenten
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;                                          // Polygon gefüllt oder als Drahtgitter
    rasterizer.lineWidth = 1.0f;                                                            // Dicke der Linien                             
    rasterizer.cullMode = graphicPipelineInfo.cullMode;                                     // Culling-Modus (z. B. keine Rückseiten oder Vorderseiten rendern)
    rasterizer.frontFace = graphicPipelineInfo.frontFace;                                   // Definiert die Ausrichtung der Vorderseite von Polygonen (im Uhrzeigersinn oder gegen den Uhrzeigersinn)
    rasterizer.depthBiasEnable = VK_FALSE;

    // Konfiguration der Multisampling-Stufe: Konfiguriert Multisampling (Anti-Aliasing)
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;                                           // Aktiviert oder deaktiviert Sample-Shading
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;                             // Anzahl der Samples pro Fragment

    // Konfiguration der Tiefen- und Stencil-Tests: Definiert Tiefentest- und Schreibregeln
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;                                                  // Aktiviert oder deaktiviert den Tiefentest
    depthStencil.depthWriteEnable = VK_TRUE;                                                 // Aktiviert oder deaktiviert das Schreiben in den Tiefenpuffer                                
    depthStencil.depthCompareOp = graphicPipelineInfo.depthCompare;                          // Definiert die Vergleichsfunktion, die für den Tiefentest verwendet wird
    depthStencil.depthBoundsTestEnable = VK_FALSE;                                           // Aktiviert oder deaktiviert den Tiefenbereichstest
    depthStencil.stencilTestEnable = VK_FALSE;                                               // Aktiviert oder deaktiviert den Stencil-Test

    // Konfiguration der Farb-Misch-Stufe: Steuert, wie Farben in den Framebuffer geschrieben werden
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};                                                                                 
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
                                            VK_COLOR_COMPONENT_G_BIT | 
                                            VK_COLOR_COMPONENT_B_BIT | 
                                            VK_COLOR_COMPONENT_A_BIT;                       // Maske, die festlegt, welche Farbkanäle (Rot, Grün, Blau, Alpha) in den Framebuffer geschrieben werden.
    colorBlendAttachment.blendEnable = VK_FALSE;                                            // Aktiviert oder deaktiviert das Mischen von Farben

    
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &DescriptorPool::getDescriptorSetLayout();

    if (vkCreatePipelineLayout(LogicalDeviceWrapper::getVkDevice(), &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState; 
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = RenderPass::getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(LogicalDeviceWrapper::getVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(LogicalDeviceWrapper::getVkDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(LogicalDeviceWrapper::getVkDevice(), vertShaderModule, nullptr);
}

VkShaderModule GraphicPipeline::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(LogicalDeviceWrapper::getVkDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("GRAPHICSPIPELINE::CREATESHADERMODULE::141 Failed to create shader module!");
    }

    return shaderModule;
}
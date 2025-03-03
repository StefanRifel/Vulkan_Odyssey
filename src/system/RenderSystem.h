#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <map>
#include <string>
#include "../base/GraphicPipeline.h"
#include "../SceneNode.h"

class RenderSystem {

private:
    std::map<std::string ,GraphicPipeline*> graphicPipelines;
    DescriptorPool descriptorPool;

public:
    RenderSystem(RenderPass& renderPass) {
        descriptorPool.createDescriptorSetLayout();

        // Hier werden die verschiedenen GraphicPipelines erstellt
        graphicPipelines.insert({"default", new GraphicPipeline("shaders/shader.vert.spv", "shaders/shader.frag.spv", 
            GraphicPipeline::getDefaultGraphicPipelineInfo(), renderPass, descriptorPool.getDescriptorSetLayout())});
        graphicPipelines.insert({"red", new GraphicPipeline("shaders/shader_red.vert.spv", "shaders/shader_red.frag.spv", 
            GraphicPipeline::getDefaultGraphicPipelineInfo(), renderPass, descriptorPool.getDescriptorSetLayout())});

        GraphicPipelineInfo graphicPipelineInfoPlane;
        graphicPipelineInfoPlane.cullMode = VK_CULL_MODE_BACK_BIT;
        graphicPipelineInfoPlane.frontFace = VK_FRONT_FACE_CLOCKWISE;
        graphicPipelineInfoPlane.depthCompare = VK_COMPARE_OP_LESS;
        graphicPipelines.insert({"plane", new GraphicPipeline("shaders/shader.vert.spv", "shaders/shader.frag.spv", 
            graphicPipelineInfoPlane, renderPass, descriptorPool.getDescriptorSetLayout())});
        
        GraphicPipelineInfo graphicPipelineInfoSkybox;
        graphicPipelineInfoSkybox.cullMode = VK_CULL_MODE_FRONT_BIT;
        graphicPipelineInfoSkybox.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        graphicPipelineInfoSkybox.depthCompare = VK_COMPARE_OP_LESS_OR_EQUAL;
        graphicPipelines.insert({"skybox", new GraphicPipeline("shaders/shader_skybox.vert.spv", "shaders/shader_skybox.frag.spv", 
            graphicPipelineInfoSkybox, renderPass, descriptorPool.getDescriptorSetLayout())});
    };

    ~RenderSystem() {
        for (auto& graphicPipeline : graphicPipelines) {
            delete graphicPipeline.second;
        }
        descriptorPool.cleanupDescriptorPool();
        descriptorPool.cleanupDescriptorSetLayout();
    }

    std::map<std::string, GraphicPipeline*>& getGraphicPipelines() {
        return graphicPipelines;
    }

    DescriptorPool& getDescriptorPool() {
        return descriptorPool;
    }

    void createDescriptorPool(int sceneNodeCount) {
        descriptorPool.createDescriptorPool(sceneNodeCount);
    }

    void renderObject(SceneNode* rootNode);
};

#endif
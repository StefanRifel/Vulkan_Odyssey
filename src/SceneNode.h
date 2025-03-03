#pragma once

#include "base/GraphicPipeline.h"
#include "renderable/Mesh.h"
#include "base/SwapChain.h"
#include <string>
#include <memory>
#include <map>

class SwapChain;

class SceneNode {
private:
    std::vector<SceneNode*> children;
    Mesh* mesh;
    SceneNode* parent;
    std::string pipelineName;

    glm::mat4 localTransform;
    glm::mat4 worldTransform;

public:
    SceneNode(Mesh* mesh = nullptr, const std::string& pipeline = "default");

    void addChild(SceneNode* child);
    std::vector<SceneNode*> getChildren();
    std::string getShaderType();

    void setLocalTransform(const glm::mat4& transform);
    void updateWorldTransform(const glm::mat4& parentTransform = glm::mat4(1.0f));
    
    void draw(VkCommandBuffer& commandBuffer, SwapChain* swapChain, std::map<std::string, GraphicPipeline*>& pipelines, uint32_t currentFrame, Camera& camera);
};
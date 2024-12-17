#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, const std::string& pipeline) 
    : mesh(mesh), parent(nullptr), pipelineName(pipeline) {
}

void SceneNode::addChild(SceneNode* child) {
    child->parent = this;
    children.push_back(child);
}

void SceneNode::draw(VkCommandBuffer& commandBuffer, std::map<std::string, GraphicsPipeline>& pipelines, uint32_t currentFrame, Camera& camera) {
    if (mesh) {
        mesh->updateUniformBuffer(camera, currentFrame);
        mesh->draw(commandBuffer, pipelines[pipelineName], currentFrame);
    }
    
    for (auto& child : children) {
        child->draw(commandBuffer, pipelines, currentFrame, camera);
    }
}
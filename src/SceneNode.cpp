#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, const std::string& pipeline) 
    : mesh(mesh), parent(nullptr), pipelineName(pipeline) {
    localTransform = glm::mat4(1.0f);
    worldTransform = glm::mat4(1.0f);
}

void SceneNode::addChild(SceneNode* child) {
    child->parent = this;
    children.push_back(child);
}

std::vector<SceneNode*> SceneNode::getChildren() {
    return children;
}

std::string SceneNode::getShaderType() {
    return pipelineName;
}

void SceneNode::setLocalTransform(const glm::mat4& transform) {
    localTransform = transform;
    updateWorldTransform();
}

void SceneNode::updateWorldTransform(const glm::mat4& parentTransform) {
    worldTransform = parentTransform * localTransform;
    
    for (auto& child : children) {
        child->updateWorldTransform(worldTransform);
    }
}

void SceneNode::draw(VkCommandBuffer& commandBuffer, std::map<std::string, GraphicPipeline*>& pipelines, uint32_t currentFrame, Camera& camera) {
    if (mesh) {
        mesh->updateUniformBuffer(camera, currentFrame, worldTransform);
        mesh->updatePushConstants(commandBuffer, pipelines[pipelineName]);
        mesh->draw(commandBuffer, pipelines[pipelineName], currentFrame);
    }
    
    for (auto& child : children) {
        child->draw(commandBuffer, pipelines, currentFrame, camera);
    }
}
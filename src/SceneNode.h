#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include "base/GraphicPipeline.h"
#include "renderable/Mesh.h"
#include <string>
#include <memory>
#include <map>

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
    
    void draw(VkCommandBuffer& commandBuffer, std::map<std::string, GraphicPipeline*>& pipelines, uint32_t currentFrame, Camera& camera);
};

#endif
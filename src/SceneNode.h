#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

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

public:
    SceneNode(Mesh* mesh = nullptr, const std::string& pipeline = "default");

    void addChild(SceneNode* child);
    
    void draw(VkCommandBuffer& commandBuffer, std::map<std::string, GraphicsPipeline>& pipelines, uint32_t currentFrame, Camera& camera);
};

#endif
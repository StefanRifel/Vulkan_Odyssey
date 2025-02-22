#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../base/DescriptorPool.h"
#include "../base/RenderPass.h"
#include "../Camera.h"

#include "../buffer/Buffer.h"
#include "../types/TransformPushConstantData.h"

#include "../base/RenderPass.h"
#include "../base/GraphicPipeline.h"
#include "../types/UniformBufferObject.h"
#include "../base/SwapChain.h"

class Mesh {
private:
    VertexBuffer vertexBuffer;

    IndexBuffer indexBuffer;

    UniformBuffer uniformBuffer;

    std::string modelPath;
    std::string texturePath;
    std::vector<std::string> texturePaths;

public:
    bool isCubeMap = false;

    Mesh(std::string modelPath, std::string texturePath);
    Mesh(std::string modelPath, std::vector<std::string>& texturePaths);
    Mesh(float width, float depth, std::string texturePath);
    ~Mesh();

    void initBuffers();
    void createTextures();
    void createCubeMapTextures();
    void createPlane(float width, float depth);
    void draw(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, uint32_t currentImage, glm::mat4& worldTransform);
    void updatePushConstants(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline, uint32_t currentFrame);
    void cleanupTextures();
};

#endif
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../base/GraphicPipeline.h"
#include "../Camera.h"

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

    void initBuffers(DescriptorPool& descriptorPool);
    void createTextures();
    void createCubeMapTextures();
    void createPlane(float width, float depth);
    void draw(VkCommandBuffer& commandBuffer, SwapChain& SwapChain, GraphicPipeline* graphicsPipeline, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, SwapChain& swapChain, uint32_t currentImage, glm::mat4& worldTransform);
    void updatePushConstants(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline);
    void cleanupTextures();
};

#endif
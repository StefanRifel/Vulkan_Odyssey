#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <random>

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

    TransformPushConstantData pushConstants{};

public:
    bool isCubeMap = false;
    bool isPlant = false;

    Mesh(std::string modelPath, std::string texturePath, bool isPlant = false) : modelPath(modelPath), texturePath(texturePath), isPlant{isPlant} {
        ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath);
        Logger::log("Model loaded: " + modelPath + " with " + std::to_string(vertexBuffer.vertices.size()) + " vertices and " + std::to_string(indexBuffer.indices.size()) + " indices.");

        if(isPlant) {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(-1.0f, 1.0f); // Bereich für Offsets
    
            pushConstants.offset = glm::vec2(dis(gen), 0.1);
        } else {
            pushConstants.offset = glm::vec2(0.0f, 0.0f);
        }
    }

    Mesh(std::string modelPath, std::vector<std::string>& texturePaths);
    Mesh(std::string texturePath);
    ~Mesh();

    void initBuffers(DescriptorPool& descriptorPool, VkCommandPool& commandPool);
    void createTextures(VkCommandPool& commandPool);
    void createCubeMapTextures(VkCommandPool& commandPool);
    void createPlane();
    void draw(VkCommandBuffer& commandBuffer, SwapChain* SwapChain, GraphicPipeline* graphicsPipeline, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, SwapChain* swapChain, uint32_t currentImage, glm::mat4& worldTransform);
    void updatePushConstants(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline);
    void cleanupTextures();
};

#endif
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../buffer/UniformBuffer.h"
#include "../base/Descriptor.h"
#include "../base/RenderPass.h"
#include "../Camera.h"

#include "../buffer/Buffer.h"

class Mesh {
private:
    std::vector<Vertex> vertices;
    Buffer vertexBuffer;

    std::vector<uint32_t> indices;
    Buffer indexBuffer;

    std::vector<Buffer> uniformBuffers;

    std::vector<VkDescriptorSet> descriptorSets;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    VkImageView textureImageView;
    VkSampler textureSampler;

    ModelLoader modelLoader;
    std::string modelPath;
    std::string texturePath;

public:
    Mesh(std::string modelPath, std::string texturePath);
    ~Mesh();

    std::vector<VkDescriptorSet>& getDescriptorSets();

    void initBuffers();
    void createTextures();
    void draw(VkCommandBuffer& commandBuffer, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, uint32_t currentImage);
    void cleanupTextures();
};

#endif
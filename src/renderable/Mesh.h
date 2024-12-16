#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../buffer/UniformBuffer.h"
#include "../base/DescriptorPool.h"
#include "../base/RenderPass.h"
#include "../Camera.h"

#include "../buffer/Buffer.h"
#include "../types/Texture.h"

class Mesh {
private:
    VertexBuffer vertexBuffer;

    IndexBuffer indexBuffer;

    std::vector<Buffer> uniformBuffers;
    std::vector<VkDescriptorSet> descriptorSets;

    Texture texture;

    std::string modelPath;
    std::string texturePath;

public:
    Mesh(std::string modelPath, std::string texturePath);
    ~Mesh();

    std::vector<VkDescriptorSet>& getDescriptorSets();

    void initBuffers();
    void createTextures();
    void draw(VkCommandBuffer& commandBuffer, uint32_t currentFrame, uint32_t imageIndex);
    void updateUniformBuffer(Camera& camera, uint32_t currentImage);
    void cleanupTextures();
};

#endif
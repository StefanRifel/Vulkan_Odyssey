#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../buffer/VertexBuffer.h"
#include "../buffer/IndexBuffer.h"
#include "../buffer/UniformBuffer.h"
#include "../base/Descriptor.h"
#include "../base/RenderPass.h"
#include "../Camera.h"

class Mesh {
private:
    std::vector<Vertex> vertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    std::vector<uint32_t> indices;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    

    std::vector<VkDescriptorSet> descriptorSets;

    ModelLoader modelLoader;
    std::string modelPath;
public:
    Mesh(std::string modelPath);
    ~Mesh();

    std::vector<uint32_t>& getIndices();
    VkBuffer& getVertexBuffer();
    VkBuffer& getIndexBuffer();
    std::vector<VkBuffer>& getUniformBuffers();
    std::vector<VkDeviceMemory>& getUniformBuffersMemory();
    std::vector<VkDescriptorSet>& getDescriptorSets();

    void initBuffers();
    void draw(VkCommandBuffer& commandBuffer, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, uint32_t currentImage);
};
#endif
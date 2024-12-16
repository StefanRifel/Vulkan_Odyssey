#ifndef BUFFER_H
#define BUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"

#include "BufferWrapper.h"
#include "../types/Texture.h"

struct Buffer {
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    VkDeviceSize bufferSize;
};

struct VertexBuffer {
    std::vector<Vertex> vertices;
    Buffer bufferData;
};

struct IndexBuffer {
    std::vector<uint32_t> indices;
    Buffer bufferData;
};

struct UniformBuffer {
    std::vector<Buffer> bufferData;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<void*> uniformBuffersMapped;
    Texture texture;
};

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Buffer& buffer);
void copyBuffer(Buffer srcBuffer, Buffer dstBuffer, VkDeviceSize size);

void createVertexBuffer(VertexBuffer& vertexBuffer);
void createIndexBuffer(IndexBuffer& indexBuffer);
void createUniformBuffers(VkDeviceSize bufferSize, UniformBuffer& uniformBuffer);

void cleanupBuffer(Buffer& buffer);

#endif
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../wrapper/LogicalDeviceWrapper.h"
#include "../base/SwapChain.h"
#include "../base/Renderer.h"
#include "../types/Texture.h"
#include "../types/Vertex.h"

#include "../base/DescriptorPool.h"

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
void copyBuffer(Buffer srcBuffer, Buffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool);

void createVertexBuffer(VertexBuffer& vertexBuffer, VkCommandPool& commandPool);
void createIndexBuffer(IndexBuffer& indexBuffer, VkCommandPool& commandPool);
void createUniformBuffers(VkDeviceSize bufferSize, UniformBuffer& uniformBuffer);

void cleanupBuffer(Buffer& buffer);

VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool);
void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool);

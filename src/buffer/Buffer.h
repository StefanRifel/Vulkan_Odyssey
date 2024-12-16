#ifndef BUFFER_H
#define BUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"

#include "BufferWrapper.h"

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

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Buffer& buffer);
void copyBuffer(Buffer srcBuffer, Buffer dstBuffer, VkDeviceSize size);

void createVertexBuffer(VertexBuffer& vertexBuffer);
void createIndexBuffer(IndexBuffer& indexBuffer);

void cleanupBuffer(Buffer& buffer);

#endif
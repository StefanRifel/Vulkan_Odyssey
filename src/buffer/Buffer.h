#ifndef BUFFER_H
#define BUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"

#include "BufferWrapper.h"

struct Buffer {
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
};

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Buffer& buffer);
void copyBuffer(Buffer srcBuffer, Buffer dstBuffer, VkDeviceSize size);

Buffer createVertexBuffer(std::vector<Vertex>& vertices);
Buffer createIndexBuffer(std::vector<uint32_t>& indices);

void cleanupBuffer(Buffer& buffer);

#endif
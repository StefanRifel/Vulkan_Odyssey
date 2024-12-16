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

Buffer createVertexBuffer(std::vector<Vertex>& vertices) {
    Buffer vertexBuffer;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer.buffer, vertexBuffer.bufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer.buffer, bufferSize);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, nullptr);

    return vertexBuffer;
}

Buffer createIndexBuffer(std::vector<uint32_t>& indices) {
    Buffer indexBuffer;

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer.buffer, indexBuffer.bufferMemory);

    copyBuffer(stagingBuffer, indexBuffer.buffer, bufferSize);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, nullptr);

    return indexBuffer;
}

#endif
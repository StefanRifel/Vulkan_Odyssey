#include "VertexBuffer.h"

VkBuffer VertexBuffer::vertexBuffer = VK_NULL_HANDLE;
VkDeviceMemory VertexBuffer::vertexBufferMemory = VK_NULL_HANDLE;

VkBuffer& VertexBuffer::getVertexBuffer() {
    return vertexBuffer;
}

void VertexBuffer::createVertexBuffer(std::vector<Vertex>& vertices) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, nullptr);
}

void VertexBuffer::cleanup() {
    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), vertexBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), vertexBufferMemory, nullptr);
}
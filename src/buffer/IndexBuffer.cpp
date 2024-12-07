#include "IndexBuffer.h"

VkBuffer IndexBuffer::indexBuffer = VK_NULL_HANDLE;
VkDeviceMemory IndexBuffer::indexBufferMemory = VK_NULL_HANDLE;

VkBuffer& IndexBuffer::getIndexBuffer() {
    return indexBuffer;
}

VkDeviceMemory& IndexBuffer::getIndexBufferMemory() {
    return indexBufferMemory;
}

void IndexBuffer::createIndexBuffer(std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), stagingBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), stagingBufferMemory, nullptr);
}
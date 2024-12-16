#include "Buffer.h"

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, Buffer& buffer) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(LogicalDeviceWrapper::getVkDevice(), &bufferInfo, nullptr, &buffer.buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(LogicalDeviceWrapper::getVkDevice(), buffer.buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = PhysicalDeviceWrapper::findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(LogicalDeviceWrapper::getVkDevice(), &allocInfo, nullptr, &buffer.bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(LogicalDeviceWrapper::getVkDevice(), buffer.buffer, buffer.bufferMemory, 0);
}

void copyBuffer(Buffer srcBuffer, Buffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = BufferUtils::beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer.buffer, dstBuffer.buffer, 1, &copyRegion);

    BufferUtils::endSingleTimeCommands(commandBuffer);
}

Buffer createVertexBuffer(std::vector<Vertex>& vertices) {
    Buffer vertexBuffer;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer stagingBuffer;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    cleanupBuffer(stagingBuffer);

    return vertexBuffer;
}

Buffer createIndexBuffer(std::vector<uint32_t>& indices) {
    Buffer indexBuffer;

    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    Buffer stagingBuffer;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (ssize_t) bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    cleanupBuffer(stagingBuffer);

    return indexBuffer;
}

void cleanupBuffer(Buffer& buffer) {
    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), buffer.buffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), buffer.bufferMemory, nullptr);
}

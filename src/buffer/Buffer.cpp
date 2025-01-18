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

void createVertexBuffer(VertexBuffer& vertexBuffer) {
    vertexBuffer.bufferData.bufferSize = sizeof(vertexBuffer.vertices[0]) * vertexBuffer.vertices.size();

    /*
        Ein stagingBuffer ist ein temporärer Vulkan-Puffer, 
        der hauptsächlich für die Übertragung von Daten zwischen der CPU 
        und der GPU verwendet wird. Er dient dazu, Daten wie 
        Texturen, Vertex-Daten oder Uniform-Daten effizient in den GPU-Speicher zu kopieren.
    */
    Buffer stagingBuffer;
    createBuffer(vertexBuffer.bufferData.bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory, 0, vertexBuffer.bufferData.bufferSize, 0, &data);
    memcpy(data, vertexBuffer.vertices.data(), (ssize_t) vertexBuffer.bufferData.bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory);

    createBuffer(vertexBuffer.bufferData.bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer.bufferData);

    copyBuffer(stagingBuffer, vertexBuffer.bufferData, vertexBuffer.bufferData.bufferSize);

    cleanupBuffer(stagingBuffer);
}

void createIndexBuffer(IndexBuffer& indexBuffer) {
    indexBuffer.bufferData.bufferSize = sizeof(indexBuffer.indices[0]) * indexBuffer.indices.size();

    Buffer stagingBuffer;
    createBuffer(indexBuffer.bufferData.bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer);

    void* data;
    vkMapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory, 0, indexBuffer.bufferData.bufferSize, 0, &data);
    memcpy(data, indexBuffer.indices.data(), (ssize_t) indexBuffer.bufferData.bufferSize);
    vkUnmapMemory(LogicalDeviceWrapper::getVkDevice(), stagingBuffer.bufferMemory);

    createBuffer(indexBuffer.bufferData.bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer.bufferData);

    copyBuffer(stagingBuffer, indexBuffer.bufferData, indexBuffer.bufferData.bufferSize);

    cleanupBuffer(stagingBuffer);
}

void createUniformBuffers(VkDeviceSize bufferSize, UniformBuffer& uniformBuffer) {
    
    uniformBuffer.bufferData.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffer.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer.bufferData[i]);

        vkMapMemory(LogicalDeviceWrapper::getVkDevice(), uniformBuffer.bufferData[i].bufferMemory, 0, bufferSize, 0, &uniformBuffer.uniformBuffersMapped[i]);
    }
}

void cleanupBuffer(Buffer& buffer) {
    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), buffer.buffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), buffer.bufferMemory, nullptr);
}

#include "UniformBuffer.h"

std::vector<VkBuffer> UniformBuffer::uniformBuffers;
std::vector<VkDeviceMemory> UniformBuffer::uniformBuffersMemory;
std::vector<void*> UniformBuffer::uniformBuffersMapped;

std::vector<VkBuffer>& UniformBuffer::getUniformBuffers() {
    return uniformBuffers;
}

std::vector<VkDeviceMemory>& UniformBuffer::getUniformBuffersMemory() {
    return uniformBuffersMemory;
}

std::vector<void*>& UniformBuffer::getUniformBuffersMapped() {
    return uniformBuffersMapped;
}

void UniformBuffer::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(LogicalDeviceWrapper::getVkDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

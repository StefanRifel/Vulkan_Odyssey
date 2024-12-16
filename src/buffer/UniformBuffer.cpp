#include "UniformBuffer.h"

std::vector<void*> UniformBuffer::uniformBuffersMapped;

std::vector<void*>& UniformBuffer::getUniformBuffersMapped() {
    return uniformBuffersMapped;
}

void UniformBuffer::createUniformBuffers(VkDeviceSize bufferSize, std::vector<Buffer>& uniformBuffers) {
    
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i]);

        vkMapMemory(LogicalDeviceWrapper::getVkDevice(), uniformBuffers[i].bufferMemory, 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

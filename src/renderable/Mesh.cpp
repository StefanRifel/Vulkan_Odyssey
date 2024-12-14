#include "Mesh.h"

Mesh::Mesh(std::string modelPath) : modelPath(modelPath) {
    modelLoader.loadModel(vertices, indices, modelPath);
}

Mesh::~Mesh() {
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), uniformBuffersMemory[i], nullptr);
    }

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), indexBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), indexBufferMemory, nullptr);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), vertexBuffer, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), vertexBufferMemory, nullptr);
}

std::vector<uint32_t>& Mesh::getIndices() {
    return indices;
}

VkBuffer& Mesh::getVertexBuffer() {
    return vertexBuffer;
}

VkBuffer& Mesh::getIndexBuffer() {
    return indexBuffer;
}

std::vector<VkDeviceMemory>& Mesh::getUniformBuffersMemory() {
    return uniformBuffersMemory;
}

std::vector<VkBuffer>& Mesh::getUniformBuffers() {
    return uniformBuffers;
}

std::vector<VkDescriptorSet>& Mesh::getDescriptorSets() {
    return descriptorSets;
}

void Mesh::initBuffers() {
    VertexBuffer::createVertexBuffer(vertices, vertexBuffer, vertexBufferMemory);

    IndexBuffer::createIndexBuffer(indices, indexBuffer, indexBufferMemory);

    UniformBuffer::createUniformBuffers(sizeof(UniformBufferObject), uniformBuffers, uniformBuffersMemory);

    Descriptor::createDescriptorSets(descriptorSets, uniformBuffers);
}

void Mesh::draw(VkCommandBuffer& commandBuffer) {

}
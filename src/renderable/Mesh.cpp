#include "Mesh.h"

Mesh::Mesh(std::string modelPath) : modelPath(modelPath) {
    modelLoader.loadModel(vertices, indices, modelPath);
}

Mesh::~Mesh() {
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

void Mesh::init() {
    VertexBuffer::createVertexBuffer(vertices, vertexBuffer, vertexBufferMemory);
    IndexBuffer::createIndexBuffer(indices, indexBuffer, indexBufferMemory);
}
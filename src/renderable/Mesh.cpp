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

void Mesh::draw(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RenderPass::getPipelineLayout(), 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Mesh::updateUniformBuffer(Camera& camera, uint32_t currentImage) {
    camera.look();

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = camera.getView();
    ubo.proj = camera.getPerspective();

    memcpy(UniformBuffer::getUniformBuffersMapped()[currentImage], &ubo, sizeof(ubo));
}
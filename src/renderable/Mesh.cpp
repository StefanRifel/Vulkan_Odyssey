#include "Mesh.h"

Mesh::Mesh(std::string modelPath, std::string texturePath) : modelPath(modelPath), texturePath(texturePath) {
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath);
}

Mesh::~Mesh() {
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        cleanupBuffer(uniformBuffers[i]);
    }

    cleanupBuffer(indexBuffer.bufferData);
    cleanupBuffer(vertexBuffer.bufferData);
}

std::vector<VkDescriptorSet>& Mesh::getDescriptorSets() {
    return descriptorSets;
}

void Mesh::initBuffers() {
    createVertexBuffer(vertexBuffer);
    createIndexBuffer(indexBuffer);

    UniformBuffer::createUniformBuffers(sizeof(UniformBufferObject), uniformBuffers);

    DescriptorPool::createDescriptorSets(descriptorSets, uniformBuffers, textureImageView, textureSampler);
}

void Mesh::createTextures() {
    TextureLoader::createTextureImage(texturePath, textureImage, textureImageMemory);
    textureImageView = TextureLoader::createTextureImageView(textureImage);
    TextureLoader::createTextureSampler(textureSampler);
}

void Mesh::draw(VkCommandBuffer& commandBuffer, uint32_t currentFrame) {
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.bufferData.buffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.bufferData.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, RenderPass::getPipelineLayout(), 0, 1, &descriptorSets[currentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer.indices.size()), 1, 0, 0, 0);
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

void Mesh::cleanupTextures() {
    vkDestroySampler(LogicalDeviceWrapper::getVkDevice(), textureSampler, nullptr);
    vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), textureImageView, nullptr);

    vkDestroyImage(LogicalDeviceWrapper::getVkDevice(), textureImage, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), textureImageMemory, nullptr);
}
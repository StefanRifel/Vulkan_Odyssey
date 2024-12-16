#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::string modelPath, std::string texturePath) : modelPath(modelPath), texturePath(texturePath) {
    std::cout << "Loading model: " << modelPath << std::endl;
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath);
    std::cout << "Model loaded: " << modelPath << " with " << vertexBuffer.vertices.size() << " vertices and " << indexBuffer.indices.size() << " indices." << std::endl;
}

Mesh::~Mesh() {
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        cleanupBuffer(uniformBuffer.bufferData[i]);
    }

    cleanupBuffer(indexBuffer.bufferData);
    cleanupBuffer(vertexBuffer.bufferData);
}

void Mesh::initBuffers() {
    createVertexBuffer(vertexBuffer);
    createIndexBuffer(indexBuffer);

    createUniformBuffers(sizeof(UniformBufferObject), uniformBuffer);
    DescriptorPool::createDescriptorSets(uniformBuffer);
}

void Mesh::createTextures() {
    TextureLoader::createTextureImage(texturePath, uniformBuffer.texture.image, uniformBuffer.texture.memory);
    uniformBuffer.texture.view = TextureLoader::createTextureImageView(uniformBuffer.texture.image);
    TextureLoader::createTextureSampler(uniformBuffer.texture.sampler);
}

void Mesh::draw(VkCommandBuffer& commandBuffer, GraphicsPipeline& graphicsPipeline, uint32_t currentFrame) {
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) SwapChain::getSwapChainExtent().width;
    viewport.height = (float) SwapChain::getSwapChainExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = SwapChain::getSwapChainExtent();
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.bufferData.buffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.bufferData.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &uniformBuffer.descriptorSets[currentFrame], 0, nullptr);

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

    memcpy(uniformBuffer.uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void Mesh::cleanupTextures() {
    vkDestroySampler(LogicalDeviceWrapper::getVkDevice(), uniformBuffer.texture.sampler, nullptr);
    vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), uniformBuffer.texture.view, nullptr);

    vkDestroyImage(LogicalDeviceWrapper::getVkDevice(), uniformBuffer.texture.image, nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), uniformBuffer.texture.memory, nullptr);
}
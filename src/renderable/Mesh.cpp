#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::string modelPath, std::string texturePath) : modelPath(modelPath), texturePath(texturePath) {
    std::cout << "Loading model: " << modelPath << std::endl;
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath);
    std::cout << "Model loaded: " << modelPath << " with " << vertexBuffer.vertices.size() << " vertices and " << indexBuffer.indices.size() << " indices." << std::endl;
}

Mesh::Mesh(std::string modelPath, std::vector<std::string>& texturePaths) : texturePaths(texturePaths), isCubeMap(true) {
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath); 
}

Mesh::Mesh(float width, float depth, std::string texturePath) : texturePath(texturePath) {
    createPlane(width, depth);
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
    TextureLoader::createTextureImage(texturePath, uniformBuffer);
    uniformBuffer.texture.view = TextureLoader::createTextureImageView(uniformBuffer.texture.image, uniformBuffer.texture.mipLevels);
    TextureLoader::createTextureSampler(uniformBuffer.texture.sampler, VK_SAMPLER_ADDRESS_MODE_REPEAT, uniformBuffer.texture.mipLevels);
}

void Mesh::createCubeMapTextures() {
    TextureLoader::createCubeMapImage(texturePaths, uniformBuffer);
    uniformBuffer.texture.view = TextureLoader::createCubeMapImageView(uniformBuffer.texture.image, uniformBuffer.texture.mipLevels);
    TextureLoader::createTextureSampler(uniformBuffer.texture.sampler, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, uniformBuffer.texture.mipLevels);
}

void Mesh::createPlane(float width, float depth) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float hw = width * 0.5f;
    float hd = depth * 0.5f;
    
    vertices = {
        {{-hw, 0.0f, -hd}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ hw, 0.0f, -hd}, {0.0f, 1.0f, 0.0f}, {8.0f, 0.0f}},  
        {{ hw, 0.0f,  hd}, {0.0f, 1.0f, 0.0f}, {8.0f, 8.0f}},
        {{-hw, 0.0f,  hd}, {0.0f, 1.0f, 0.0f}, {0.0f, 8.0f}}
    };

    indices = {0, 1, 2, 2, 3, 0};
    
    vertexBuffer.vertices = vertices;
    indexBuffer.indices = indices;
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

void Mesh::updateUniformBuffer(Camera& camera, uint32_t currentImage, glm::mat4& worldTransform) {
    camera.look();

    UniformBufferObject ubo{};
    ubo.model = worldTransform;
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
#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::string modelPath, std::string texturePath) : modelPath(modelPath), texturePath(texturePath) {
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath);
    Logger::log("Model loaded: " + modelPath + " with " + std::to_string(vertexBuffer.vertices.size()) + " vertices and " + std::to_string(indexBuffer.indices.size()) + " indices.");
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

void Mesh::draw(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline, uint32_t currentFrame) {
    graphicsPipeline->bind(commandBuffer);

    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.bufferData.buffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.bufferData.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getLayout(), 0, 1, &uniformBuffer.descriptorSets[currentFrame], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer.indices.size()), 1, 0, 0, 0);
}

void Mesh::updatePushConstants(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline) {
    TransformPushConstantData pushConstants{};
    pushConstants.offset = glm::vec2(0.0f, 0.0f);

    vkCmdPushConstants(commandBuffer, graphicsPipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(TransformPushConstantData), &pushConstants);
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
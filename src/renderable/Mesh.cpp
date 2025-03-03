#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::string modelPath, std::vector<std::string>& texturePaths) : texturePaths(texturePaths), isCubeMap(true) {
    ModelLoader::loadModel(vertexBuffer, indexBuffer, modelPath); 
}

Mesh::Mesh(std::string texturePath) : texturePath(texturePath) {
    createPlane();
}

Mesh::~Mesh() {
    for (int i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        cleanupBuffer(uniformBuffer.bufferData[i]);
    }

    cleanupBuffer(indexBuffer.bufferData);
    cleanupBuffer(vertexBuffer.bufferData);
}

void Mesh::initBuffers(DescriptorPool& descriptorPool, VkCommandPool& commandPool) {
    createVertexBuffer(vertexBuffer, commandPool);
    createIndexBuffer(indexBuffer, commandPool);

    createUniformBuffers(sizeof(UniformBufferObject), uniformBuffer);
    descriptorPool.createDescriptorSets(uniformBuffer);
}

void Mesh::createTextures(VkCommandPool& commandPool) {
    TextureLoader::createTextureImage(texturePath, uniformBuffer, commandPool);
    uniformBuffer.texture.view = TextureLoader::createTextureImageView(uniformBuffer.texture.image, uniformBuffer.texture.mipLevels);
    TextureLoader::createTextureSampler(uniformBuffer.texture.sampler, VK_SAMPLER_ADDRESS_MODE_REPEAT, uniformBuffer.texture.mipLevels);
}

void Mesh::createCubeMapTextures(VkCommandPool& commandPool) {
    TextureLoader::createCubeMapImage(texturePaths, uniformBuffer, commandPool);
    uniformBuffer.texture.view = TextureLoader::createCubeMapImageView(uniformBuffer.texture.image, uniformBuffer.texture.mipLevels);
    TextureLoader::createTextureSampler(uniformBuffer.texture.sampler, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE, uniformBuffer.texture.mipLevels);
}

void Mesh::createPlane() {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    const int SIZE = 512;
    const int VERTEX_COUNT = 512;

    // Define the range for random heights
    const float MIN_HEIGHT = 0.0f;
    const float MAX_HEIGHT = 0.4f;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(MIN_HEIGHT, MAX_HEIGHT);

    int vertexPointer = 0;
    for(int i=0;i<VERTEX_COUNT;i++){
        for(int j=0;j<VERTEX_COUNT;j++){
            Vertex v;
            glm::vec3 vertex;
            vertex.x = -SIZE/2.0f + SIZE*i/(float)SIZE;
            vertex.y = dis(gen);
            vertex.z = -SIZE/2.0f + SIZE*j/(float)SIZE;
            v.pos = vertex;

            glm::vec2 texture;
            texture.x = ((float)j/((float)VERTEX_COUNT - 1)) * SIZE/8;
            texture.y = ((float)i/((float)VERTEX_COUNT - 1)) * SIZE/8;
            v.texCoord = texture;

            vertices.push_back(v);
            vertexPointer++;
        }
    }

    for(int gz=0;gz<VERTEX_COUNT-1;gz++){
        for(int gx=0;gx<VERTEX_COUNT-1;gx++){
            int topLeft = (gz*VERTEX_COUNT)+gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    vertexBuffer.vertices = vertices;
    indexBuffer.indices = indices;
}

void Mesh::draw(VkCommandBuffer& commandBuffer, SwapChain* swapChain, GraphicPipeline* graphicsPipeline, uint32_t currentFrame) {
    graphicsPipeline->bind(commandBuffer, swapChain);

    VkDeviceSize offsets[] = {0};   

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.bufferData.buffer, offsets);

    vkCmdBindIndexBuffer(commandBuffer, indexBuffer.bufferData.buffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getLayout(), 0, 1, &uniformBuffer.descriptorSets[currentFrame], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indexBuffer.indices.size()), 1, 0, 0, 0);
}

void Mesh::updatePushConstants(VkCommandBuffer& commandBuffer, GraphicPipeline* graphicsPipeline) {
    vkCmdPushConstants(commandBuffer, graphicsPipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(TransformPushConstantData), &pushConstants);
}

void Mesh::updateUniformBuffer(Camera& camera, SwapChain* swapChain, uint32_t currentImage, glm::mat4& worldTransform) {
    camera.look(swapChain->getSwapChainExtent().width, swapChain->getSwapChainExtent().height);

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
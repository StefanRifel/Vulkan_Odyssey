#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../base/DescriptorPool.h"
#include "../base/RenderPass.h"
#include "../Camera.h"

#include "../buffer/Buffer.h"

#include "../base/RenderPass.h"
#include "../types/UniformBufferObject.h"
#include "../base/SwapChain.h"

class Mesh {
private:
    VertexBuffer vertexBuffer;

    IndexBuffer indexBuffer;

    UniformBuffer uniformBuffer;

    std::string modelPath;
    std::string texturePath;
    std::vector<std::string> texturePaths;

public:
    bool isCubeMap = false;

    Mesh(std::string modelPath, std::string texturePath);
    Mesh(std::string modelPath, std::vector<std::string>& texturePaths);
    ~Mesh();

    void initBuffers();
    void createTextures();
    void createCubeMapTextures();
    void draw(VkCommandBuffer& commandBuffer, GraphicsPipeline& graphicsPipeline, uint32_t currentFrame);
    void updateUniformBuffer(Camera& camera, uint32_t currentImage, glm::mat4& worldTransform);
    void cleanupTextures();
};

#endif
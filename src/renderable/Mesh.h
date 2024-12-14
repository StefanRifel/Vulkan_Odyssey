#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "../loader/ModelLoader.h"
#include "../types/Vertex.h"
#include "../buffer/VertexBuffer.h"
#include "../buffer/IndexBuffer.h"

class Mesh {
private:
    std::vector<Vertex> vertices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    std::vector<uint32_t> indices;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    ModelLoader modelLoader;
    std::string modelPath;
public:
    Mesh(std::string modelPath);
    ~Mesh();

    std::vector<uint32_t>& getIndices();
    VkBuffer& getVertexBuffer();
    VkBuffer& getIndexBuffer();

    void init();

};
#endif
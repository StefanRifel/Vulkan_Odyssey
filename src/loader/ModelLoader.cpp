#define TINYOBJLOADER_IMPLEMENTATION
#include "ModelLoader.h"

void ModelLoader::loadModel(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, std::string& modelPath) {
    tinyobj::attrib_t attrib;                         // container holds all of the positions, normals and texture coordinates in its attrib.vertices, attrib.normals and attrib.texcoords
    std::vector<tinyobj::shape_t> shapes;             // container contains all of the separate objects and their faces
    std::vector<tinyobj::material_t> materials;       // materials used in the OBJ file will be stored. Each material contains properties like color, texture, and other material attributes
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertexBuffer.vertices.size());
                vertexBuffer.vertices.push_back(vertex);
            }

            indexBuffer.indices.push_back(uniqueVertices[vertex]);
        }
    }
}

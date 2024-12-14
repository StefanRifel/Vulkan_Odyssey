#include "Mesh.h"

Mesh::Mesh(std::string modelPath) : modelPath(modelPath) {
    modelLoader.loadModel(vertices, indices, modelPath);
}

Mesh::~Mesh() {

}

std::vector<uint32_t>& Mesh::getIndices() {
    return indices;
}

void Mesh::init() {
    VertexBuffer::createVertexBuffer(vertices);
    IndexBuffer::createIndexBuffer(indices);
}
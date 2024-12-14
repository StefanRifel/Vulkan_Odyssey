#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"

#include "BufferWrapper.h"

class VertexBuffer {
public:
    static void createVertexBuffer(std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
};

#endif

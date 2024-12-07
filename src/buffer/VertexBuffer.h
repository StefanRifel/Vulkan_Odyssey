#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/Vertex.h"

#include "BufferWrapper.h"

class VertexBuffer {
    
private:
    static VkBuffer vertexBuffer;
    static VkDeviceMemory vertexBufferMemory;

public:
    static VkBuffer& getVertexBuffer();

    static void createVertexBuffer(std::vector<Vertex>& vertices);
    
    static void cleanup();
};

#endif

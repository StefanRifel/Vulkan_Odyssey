#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "BufferWrapper.h"

#include "../wrapper/LogicalDeviceWrapper.h"

class IndexBuffer {
public:
    static void createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
};

#endif
#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "BufferWrapper.h"

#include "../wrapper/LogicalDeviceWrapper.h"

class IndexBuffer {
    
private:
    static VkBuffer indexBuffer;
    static VkDeviceMemory indexBufferMemory;

public:
    static VkBuffer& getIndexBuffer();
    static VkDeviceMemory& getIndexBufferMemory();

    static void createIndexBuffer(std::vector<uint32_t>& indices);

};

#endif
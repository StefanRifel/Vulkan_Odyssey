#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/UniformBufferObject.h"
#include "../types/DefaultValues.h"
#include "BufferWrapper.h"

#include <vector>

class UniformBuffer {

private:
    static std::vector<VkBuffer> uniformBuffers;
    static std::vector<VkDeviceMemory> uniformBuffersMemory;
    static std::vector<void*> uniformBuffersMapped;

public:
    static std::vector<VkBuffer>& getUniformBuffers();
    static std::vector<VkDeviceMemory>& getUniformBuffersMemory();
    static std::vector<void*>& getUniformBuffersMapped();

    static void createUniformBuffers();
};

#endif
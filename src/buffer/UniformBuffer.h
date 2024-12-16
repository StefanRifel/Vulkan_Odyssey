#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/UniformBufferObject.h"
#include "../types/DefaultValues.h"
#include "Buffer.h"

#include <vector>

class UniformBuffer {
public:
    static void createUniformBuffers(VkDeviceSize bufferSize, std::vector<Buffer>& uniformBuffers, std::vector<void*>& uniformBuffersMapped);
};

#endif
#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/UniformBufferObject.h"
#include "../types/DefaultValues.h"
#include "Buffer.h"

#include <vector>

class UniformBuffer {
private:
    static std::vector<void*> uniformBuffersMapped;

public:
    static std::vector<void*>& getUniformBuffersMapped();

    static void createUniformBuffers(VkDeviceSize bufferSize, std::vector<Buffer>& uniformBuffers);
};

#endif
#ifndef LOGICALDEVICEWRAPPER_H
#define LOGICALDEVICEWRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/QueueFamilyIndices.h"

#include "PhysicalDeviceWrapper.h"
#include "InstanceWrapper.h"

#include <vector>
#include <set>
#include <stdexcept>

class LogicalDeviceWrapper {

private:
    static VkDevice device;
    static VkQueue graphicsQueue;
    static VkQueue presentQueue;

public:
    static VkDevice& getVkDevice();
    static VkQueue& getGraphicsQueue();
    static VkQueue& getPresentQueue();

    static void createLogicalDevice();
};

#endif
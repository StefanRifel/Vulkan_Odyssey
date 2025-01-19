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

/**
 * @brief Wrapper-Klasse für die Verwaltung eines Vulkan Logical Devices.
 * 
 * Diese Klasse abstrahiert die Erstellung, Nutzung und Bereinigung eines Logical Devices sowie
 * der zugehörigen Queues (Graphics und Present Queues) in Vulkan.
 */
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

    static void cleanup();
};

#endif
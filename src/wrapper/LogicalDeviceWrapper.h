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
 * Wrapper-Klasse für das Vulkan Logical Device, das die Kommunikation mit dem GPU
 * und die Verwaltung von Queues für Grafik- und Präsentationsoperationen übernimmt.
 */
class LogicalDeviceWrapper {

private:
    // Komunikation mit dem GPU 
    static VkDevice device;
    // Die Queue, die für Grafikoperationen zuständig ist (z.B. Rendering).
    static VkQueue graphicsQueue;
    // Die Queue, die für die Präsentation von Bildern auf dem Bildschirm zuständig ist.
    static VkQueue presentQueue;

public:
    static VkDevice& getVkDevice();
    static VkQueue& getGraphicsQueue();
    static VkQueue& getPresentQueue();

    static void createLogicalDevice();

    static void cleanup();
};

#endif
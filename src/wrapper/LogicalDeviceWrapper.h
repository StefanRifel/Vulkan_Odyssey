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
    /*
        Ein VkDevice in Vulkan repräsentiert ein logisches Gerät, 
        as als abstrakte Schnittstelle zwischen der Anwendung und 
        einem ausgewählten VkPhysicalDevice (also der physischen GPU) dient. 
        Während das VkPhysicalDevice die Hardware beschreibt, ermöglicht das 
        VkDevice der Anwendung, direkt mit dieser Hardware zu arbeiten, indem 
        es Ressourcen, Warteschlangen und andere Operationen bereitstellt.
    */
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
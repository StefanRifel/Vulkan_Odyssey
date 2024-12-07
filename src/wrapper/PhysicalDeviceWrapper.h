#ifndef PHYSICALDEVICEWRAPPER_H
#define PHYSICALDEVICEWRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/QueueFamilyIndices.h"
#include "../types/SwapChainSupportDetails.h"
#include "InstanceWrapper.h"

#include <vector>
#include <set>

class PhysicalDeviceWrapper {

private:
    static VkPhysicalDevice physicalDevice;
    const static std::vector<const char*> deviceExtensions;
    
public:
    static void pickPhysicalDevice();

    static bool isDeviceSuitable(VkPhysicalDevice device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    static VkPhysicalDevice& getPhysicalDevice();
    static const std::vector<const char*>& getDeviceExtensions();

    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

#endif
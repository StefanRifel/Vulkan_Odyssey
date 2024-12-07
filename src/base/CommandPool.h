#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/QueueFamilyIndices.h"
#include "../types/DefaultValues.h"

#include "../wrapper/PhysicalDeviceWrapper.h"
#include "../wrapper/LogicalDeviceWrapper.h"

class CommandPool {

private:
    static VkCommandPool commandPool;
    static std::vector<VkCommandBuffer> commandBuffers;

public:
    static VkCommandPool& getCommandPool(); 
    static std::vector<VkCommandBuffer>& getCommandBuffers();

    static void createCommandPool();
    static void createCommandBuffers();
};

#endif 
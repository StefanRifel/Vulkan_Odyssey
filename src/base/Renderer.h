#ifndef RENDERER_H
#define RENDERER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "SwapChain.h"

class Renderer {

private:
    std::vector<VkCommandBuffer> commandBuffers;
    static VkCommandPool commandPool;

    void createCommandBuffers();
    void createCommandPool();

public:
    Renderer() {
        createCommandPool();
        createCommandBuffers();
    }

    std::vector<VkCommandBuffer>& getCommandBuffers() {
        return commandBuffers;
    };

    VkCommandPool& getCommandPool() {
        return commandPool;
    }

    static VkCommandBuffer beginSingleTimeCommands();
    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);
}; 

#endif
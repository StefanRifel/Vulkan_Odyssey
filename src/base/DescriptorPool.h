#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../buffer/Buffer.h"
#include "../types/UniformBufferObject.h"

struct UniformBuffer;

class DescriptorPool {
    
private:
    VkDevice device;
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

public:
    DescriptorPool()
    : device(VK_NULL_HANDLE), descriptorPool(VK_NULL_HANDLE), descriptorSetLayout(VK_NULL_HANDLE) {}

    void cleanupDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(LogicalDeviceWrapper::getVkDevice(), descriptorSetLayout, nullptr);
    }
    
    void cleanupDescriptorPool() {
        vkDestroyDescriptorPool(LogicalDeviceWrapper::getVkDevice(), descriptorPool, nullptr);
    }

    VkDescriptorSetLayout& getDescriptorSetLayout() {
        return descriptorSetLayout;
    };
    VkDescriptorPool& getDescriptorPool() {
        return descriptorPool;
    };

    void createDescriptorPool(int sceneNodeCount);
    void createDescriptorSetLayout();
    void createDescriptorSets(UniformBuffer& uniformBuffer);
};
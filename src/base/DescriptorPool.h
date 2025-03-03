#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../buffer/Buffer.h"
#include "../types/UniformBufferObject.h"

class DescriptorPool {
    
private:
    VkDescriptorPool descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

public:

    VkDescriptorSetLayout& getDescriptorSetLayout() {
        return descriptorSetLayout;
    };
    VkDescriptorPool& getDescriptorPool() {
        return descriptorPool;
    };

    void createDescriptorPool(int sceneNodeCount);
    void createDescriptorSetLayout();
    void createDescriptorSets(UniformBuffer& uniformBuffer);

    void cleanupDescriptorPool() {
        vkDestroyDescriptorPool(LogicalDeviceWrapper::getVkDevice(), descriptorPool, nullptr);
    };
    void cleanupDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(LogicalDeviceWrapper::getVkDevice(), descriptorSetLayout, nullptr);
    };
};

#endif
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

    void cleanup() {
        if (descriptorSetLayout != VK_NULL_HANDLE) {
            vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
            descriptorSetLayout = VK_NULL_HANDLE;
        }

        if (descriptorPool != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);
            descriptorPool = VK_NULL_HANDLE;
        }
    }

public:
    DescriptorPool() noexcept
    : device(VK_NULL_HANDLE), descriptorPool(VK_NULL_HANDLE), descriptorSetLayout(VK_NULL_HANDLE) {}

    explicit DescriptorPool(VkDevice logicalDevice) 
        : device(logicalDevice), descriptorPool(VK_NULL_HANDLE), descriptorSetLayout(VK_NULL_HANDLE) {}


    ~DescriptorPool() {
        std::cout << "Destroying DescriptorPool" << std::endl;
        cleanup();
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
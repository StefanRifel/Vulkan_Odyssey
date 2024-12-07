#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/DefaultValues.h"
#include "../wrapper/LogicalDeviceWrapper.h"
#include "../loader/TextureLoader.h"
#include "../buffer/UniformBuffer.h"

#include <vector>

class Descriptor {
    
private:
    static VkDescriptorSetLayout descriptorSetLayout;
    static VkDescriptorPool descriptorPool;
    static std::vector<VkDescriptorSet> descriptorSets;

public:
    static VkDescriptorSetLayout& getDescriptorSetLayout();
    static std::vector<VkDescriptorSet>& getDescriptorSets();
    static VkDescriptorPool& getDescriptorPool();

    static void createDescriptorPool();
    static void createDescriptorSets();
};

#endif
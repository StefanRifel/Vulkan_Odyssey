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

public:
    static VkDescriptorSetLayout& getDescriptorSetLayout();
    static VkDescriptorPool& getDescriptorPool();

    static void createDescriptorPool();
    static void createDescriptorSetLayout();
    static void createDescriptorSets(std::vector<VkDescriptorSet>& descriptorSets, std::vector<Buffer>& uniformBuffers, VkImageView& textureImageView, VkSampler& textureSampler);
};

#endif
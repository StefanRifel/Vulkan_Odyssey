#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/DefaultValues.h"
#include "../wrapper/LogicalDeviceWrapper.h"
#include "../loader/TextureLoader.h"
#include "../types/Texture.h"

#include <vector>

#include "../types/UniformBufferObject.h"   

class DescriptorPool {
    
private:
    static VkDescriptorPool descriptorPool;
    static VkDescriptorSetLayout descriptorSetLayout;

public:
    static VkDescriptorSetLayout& getDescriptorSetLayout();
    static VkDescriptorPool& getDescriptorPool();

    static void createDescriptorPool(int sceneNodeCount);
    static void createDescriptorSetLayout();
    static void createDescriptorSets(UniformBuffer& uniformBuffer);
};

#endif
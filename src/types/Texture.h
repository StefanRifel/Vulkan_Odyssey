#ifndef TEXTURE_H
#define TEXTURE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct Texture {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkSampler sampler = VK_NULL_HANDLE;
};

#endif
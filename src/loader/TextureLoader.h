#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../library/stb_image.h"

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../buffer/VertexBuffer.h"

class TextureLoader {

private:
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

public:
    static void createTextureImage(std::string texturePath, VkImage& textureImage, VkDeviceMemory& textureImageMemory);
    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    
    static void transitionImageLayout(VkImage image, [[maybe_unused]] VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    static VkImageView createTextureImageView(VkImage& textureImage);
    static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void createTextureSampler(VkSampler& textureSampler);
};

#endif
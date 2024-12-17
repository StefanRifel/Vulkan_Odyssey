#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../library/stb_image.h"

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../buffer/Buffer.h"

class TextureLoader {

private:
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }

public:
    static void createTextureImage(std::string texturePath, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t& mipLevels);
    static void createImage(uint32_t width, uint32_t height, uint32_t& mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    
    static void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    static void transitionImageLayout(VkImage image, [[maybe_unused]] VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t& mipLevels);
    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    static VkImageView createTextureImageView(VkImage& textureImage, uint32_t& mipLevels);
    static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t& mipLevels);

    static void createTextureSampler(VkSampler& textureSampler, uint32_t& mipLevels);
};

#endif
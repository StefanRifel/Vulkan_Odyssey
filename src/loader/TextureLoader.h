#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../library/stb_image.h"

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../buffer/VertexBuffer.h"

class TextureLoader {

private:
    static VkImage textureImage;
    static VkDeviceMemory textureImageMemory;

    static VkImageView textureImageView;
    static VkSampler textureSampler;

    static const std::string TEXTURE_PATH;

public:
    static VkImage& getTextureImage();
    static VkDeviceMemory& getTextureImageMemory();
    static VkImageView& getTextureImageView();
    static VkSampler& getTextureSampler();

    static void createTextureImage();
    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    
    static void transitionImageLayout(VkImage image, [[maybe_unused]] VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    static void createTextureImageView();
    static VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    static void createTextureSampler();
};

#endif
#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Window.h"

#include "../wrapper/LogicalDeviceWrapper.h"
#include "RenderPass.h"

#include "../loader/TextureLoader.h"

class SwapChain {

private:
    static std::vector<VkFramebuffer> swapChainFramebuffers;

    static VkImage depthImage;
    static VkDeviceMemory depthImageMemory;
    static VkImageView depthImageView;

    static VkSwapchainKHR swapChain;
    static std::vector<VkImage> swapChainImages;
    static VkFormat swapChainImageFormat;
    static VkExtent2D swapChainExtent;
    static std::vector<VkImageView> swapChainImageViews;

    static uint32_t mipLevels;

public:

    static std::vector<VkFramebuffer>& getSwapChainFramebuffers();
    static VkSwapchainKHR& getSwapChain();
    static VkFormat& getSwapChainImageFormat();
    static VkExtent2D& getSwapChainExtent();

    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);

    static void createFramebuffers();

    static void createSwapChain(Window* window);
    static void recreateSwapChain(Window* window);
    static void cleanupSwapChain();

    static void createImageViews();

    static void createDepthResources();
    static VkFormat findDepthFormat();
    static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif
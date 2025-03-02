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
    static RenderPass renderPass;

    static std::vector<VkFramebuffer> swapChainFramebuffers;

    static VkFormat swapChainImageFormat;
    

    static VkImage depthImage;
    static VkDeviceMemory depthImageMemory;
    static VkImageView depthImageView;
    static std::vector<VkImage> swapChainImages;
    static std::vector<VkImageView> swapChainImageViews;

    static VkExtent2D swapChainExtent;

    static VkSwapchainKHR swapChain;

    static uint32_t mipLevels;

    

    // Helper functions
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);

public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    // Synchronisationsobjekte
    static std::vector<VkSemaphore> imageAvailableSemaphores;
    static std::vector<VkSemaphore> renderFinishedSemaphores;
    static std::vector<VkFence> inFlightFences;
    static uint32_t currentFrame;

    ~SwapChain() {
        for (ssize_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(LogicalDeviceWrapper::getVkDevice(), inFlightFences[i], nullptr);
        }
    }

    static std::vector<VkFramebuffer>& getSwapChainFramebuffers() {
        return swapChainFramebuffers;
    };
    static VkSwapchainKHR& getSwapChain() {
        return swapChain;
    }
    static VkFormat& getSwapChainImageFormat() {
        return swapChainImageFormat;
    }
    static VkExtent2D& getSwapChainExtent() {
        return swapChainExtent;
    }
    static RenderPass& getRenderPass() {
        return renderPass;
    }

    static void init(Window* window) {
        SwapChain::createSwapChain(window);
        SwapChain::createImageViews();
        renderPass.createRenderPass();
    }

    static void createSyncObjects();
    static void createFramebuffers();
    static void createSwapChain(Window* window);
    static void createDepthResources();
    static void createImageViews();

    static void recreateSwapChain(Window* window);

    static void cleanupSwapChain();
    static void cleanupSyncObjects();
    static void cleanupRenderPass();

    static VkFormat findDepthFormat();
    static VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif
#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../Window.h"

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../loader/TextureLoader.h"

#include "RenderPass.h"

class SwapChain {

private:
    RenderPass renderPass {};

    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkFormat swapChainImageFormat;
    
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

    VkExtent2D swapChainExtent;
    VkSwapchainKHR swapChain;

    uint32_t mipLevels;


    // Create functions

    // Helper functions
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);

public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    // Synchronisationsobjekte
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame;

    SwapChain() {
        currentFrame = 0;
        mipLevels = 1;
    }

    ~SwapChain() {
        for (ssize_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(LogicalDeviceWrapper::getVkDevice(), inFlightFences[i], nullptr);
        }
    }

    std::vector<VkFramebuffer>& getSwapChainFramebuffers() {
        return swapChainFramebuffers;
    };
    VkSwapchainKHR& getSwapChain() {
        return swapChain;
    }
    VkFormat& getSwapChainImageFormat() {
        return swapChainImageFormat;
    }
    VkExtent2D& getSwapChainExtent() {
        return swapChainExtent;
    }
    RenderPass& getRenderPass() {
        return renderPass;
    }

    void init(Window* window) {
        createSwapChain(window);
        createImageViews();
        renderPass.createRenderPass(swapChainImageFormat, findDepthFormat());
        // createDescriptorSetLayout();
        createDepthResources();
        createFramebuffers();
        // createDescriptorPool();
        createSyncObjects();
    }

    void createSyncObjects();
    void createFramebuffers();
    void createSwapChain(Window* window);
    void createDepthResources();
    void createImageViews();

    void recreateSwapChain(Window* window);

    void cleanupSwapChain();
    void cleanupSyncObjects();
    void cleanupRenderPass();

    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif
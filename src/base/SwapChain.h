#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "../Window.h"

#include "../wrapper/LogicalDeviceWrapper.h"

#include "../loader/TextureLoader.h"

#include "RenderPass.h"

#include "../wrapper/LogicalDeviceWrapper.h"

class SwapChain {

private:
    Window* window;

    std::unique_ptr<RenderPass> renderPass;
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

    // Helper functions
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window);

    // Create-Funktionen
    void createSyncObjects();
    void createFramebuffers();
    void createSwapChain();
    void createDepthResources();
    void createImageViews();

public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    // Synchronisationsobjekte
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame;

    SwapChain(Window* window) : window {window}, mipLevels(1), currentFrame(0) {
        createSwapChain();
        createImageViews();
        
        renderPass = std::make_unique<RenderPass>(swapChainImageFormat, findDepthFormat());
        
        createDepthResources();
        createFramebuffers();
        createSyncObjects();
    }

    // Cleanup-Funktionen
    void cleanupSwapChain() {
        vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), depthImageView, nullptr);
        vkDestroyImage(LogicalDeviceWrapper::getVkDevice(), depthImage, nullptr);
        vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), depthImageMemory, nullptr);

        for (auto framebuffer : swapChainFramebuffers) {
            vkDestroyFramebuffer(LogicalDeviceWrapper::getVkDevice(), framebuffer, nullptr);
        }

        for (auto imageView : swapChainImageViews) {
            vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), imageView, nullptr);
        }

        vkDestroySwapchainKHR(LogicalDeviceWrapper::getVkDevice(), swapChain, nullptr);
    };

    void cleanupSyncObjects() {
        for (ssize_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(LogicalDeviceWrapper::getVkDevice(), inFlightFences[i], nullptr);
        }
    };

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
    std::unique_ptr<RenderPass>& getRenderPass() {
        return renderPass;
    }

    void recreateSwapChain();

    VkFormat findDepthFormat();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};
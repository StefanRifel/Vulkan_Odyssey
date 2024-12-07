#include "VulkanContext.hpp"

void VulkanContext::init() {
    window.initWindow();
    initVulkan();
}

void VulkanContext::initVulkan() {
    InstanceWrapper::createInstance();
    InstanceWrapper::setupDebugMessenger();
    InstanceWrapper::createSurface(window.getGLFWwindow());

    PhysicalDeviceWrapper::pickPhysicalDevice();
    LogicalDeviceWrapper::createLogicalDevice();

    SwapChain::createSwapChain(window);
    SwapChain::createImageViews();
    RenderPass::createRenderPass();
    createDescriptorSetLayout();
    RenderPass::createGraphicsPipeline();
    CommandPool::createCommandPool();
    SwapChain::createDepthResources();
    SwapChain::createFramebuffers();
    TextureLoader::createTextureImage();
    TextureLoader::createTextureImageView();
    TextureLoader::createTextureSampler();

    // Hier werden alle unsere Objekte geladen die wir in der Szene brauchen
    // Pfade zu der obj sind gerade noch hard coded
    modelLoader.loadModel(vertices, indices);

    VertexBuffer::createVertexBuffer(vertices);
    IndexBuffer::createIndexBuffer(indices);
    UniformBuffer::createUniformBuffers();
    Descriptor::createDescriptorPool();
    Descriptor::createDescriptorSets();
    CommandPool::createCommandBuffers();
    createSyncObjects();
}

void VulkanContext::mainLoop() {
    while (!glfwWindowShouldClose(window.getGLFWwindow())) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void VulkanContext::cleanup() {
    SwapChain::cleanupSwapChain();

    vkDestroyPipeline(LogicalDeviceWrapper::getVkDevice(), RenderPass::getGraphicsPipeline(), nullptr);
    vkDestroyPipelineLayout(LogicalDeviceWrapper::getVkDevice(), RenderPass::getPipelineLayout(), nullptr);
    vkDestroyRenderPass(LogicalDeviceWrapper::getVkDevice(), RenderPass::getRenderPass(), nullptr);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), UniformBuffer::getUniformBuffers()[i], nullptr);
        vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), UniformBuffer::getUniformBuffersMemory()[i], nullptr);
    }

    vkDestroyDescriptorPool(LogicalDeviceWrapper::getVkDevice(), Descriptor::getDescriptorPool(), nullptr);

    vkDestroySampler(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureSampler(), nullptr);
    vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImageView(), nullptr);

    vkDestroyImage(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImage(), nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImageMemory(), nullptr);

    vkDestroyDescriptorSetLayout(LogicalDeviceWrapper::getVkDevice(), Descriptor::getDescriptorSetLayout(), nullptr);

    vkDestroyBuffer(LogicalDeviceWrapper::getVkDevice(), IndexBuffer::getIndexBuffer(), nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), IndexBuffer::getIndexBufferMemory(), nullptr);

    VertexBuffer::cleanup();

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(LogicalDeviceWrapper::getVkDevice(), inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(LogicalDeviceWrapper::getVkDevice(), CommandPool::getCommandPool(), nullptr);

    vkDestroyDevice(LogicalDeviceWrapper::getVkDevice(), nullptr);

    if (enableValidationLayers) {
        InstanceWrapper::DestroyDebugUtilsMessengerEXT(nullptr);
    }

    vkDestroySurfaceKHR(InstanceWrapper::getInstance(), InstanceWrapper::getVkSurfaceKHR(), nullptr);
    vkDestroyInstance(InstanceWrapper::getInstance(), nullptr);

    window.cleanup();
}


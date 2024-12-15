#include "Scene.h"

Scene::Scene(Window* window) : window(window) {
}

void Scene::initVulkan() {
    InstanceWrapper::createInstance();
    InstanceWrapper::setupDebugMessenger();
    InstanceWrapper::createSurface(window->getGLFWwindow());

    PhysicalDeviceWrapper::pickPhysicalDevice();
    LogicalDeviceWrapper::createLogicalDevice();

    SwapChain::createSwapChain(window);
    SwapChain::createImageViews();
    RenderPass::createRenderPass();
    Descriptor::createDescriptorSetLayout();
    RenderPass::createGraphicsPipeline();
    CommandPool::createCommandPool();
    SwapChain::createDepthResources();
    SwapChain::createFramebuffers();
    TextureLoader::createTextureImage();
    TextureLoader::createTextureImageView();
    TextureLoader::createTextureSampler();


    Descriptor::createDescriptorPool();
    // Hier werden alle unsere Objekte geladen die wir in der Szene brauchen
    // Pfade zu der obj sind gerade noch hard coded
    mesh->initBuffers();
    
    CommandPool::createCommandBuffers();
    createSyncObjects();
}

void Scene::waitOutstandingQueues() {
    // To wait on the host for the completion of outstanding queue operations for all queues on a given logical device
    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void Scene::cleanup() {
    SwapChain::cleanupSwapChain();

    vkDestroyPipeline(LogicalDeviceWrapper::getVkDevice(), RenderPass::getGraphicsPipeline(), nullptr);
    vkDestroyPipelineLayout(LogicalDeviceWrapper::getVkDevice(), RenderPass::getPipelineLayout(), nullptr);
    vkDestroyRenderPass(LogicalDeviceWrapper::getVkDevice(), RenderPass::getRenderPass(), nullptr);

    

    vkDestroyDescriptorPool(LogicalDeviceWrapper::getVkDevice(), Descriptor::getDescriptorPool(), nullptr);

    vkDestroySampler(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureSampler(), nullptr);
    vkDestroyImageView(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImageView(), nullptr);

    vkDestroyImage(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImage(), nullptr);
    vkFreeMemory(LogicalDeviceWrapper::getVkDevice(), TextureLoader::getTextureImageMemory(), nullptr);

    vkDestroyDescriptorSetLayout(LogicalDeviceWrapper::getVkDevice(), Descriptor::getDescriptorSetLayout(), nullptr);
    
    delete mesh;

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(LogicalDeviceWrapper::getVkDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(LogicalDeviceWrapper::getVkDevice(), inFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(LogicalDeviceWrapper::getVkDevice(), CommandPool::getCommandPool(), nullptr);

    
    LogicalDeviceWrapper::cleanup();

    InstanceWrapper::cleanup();
}

Camera& Scene::getCamera() {
    return camera;
}

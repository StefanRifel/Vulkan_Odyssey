#include "Scene.hpp"

void Scene::init() {
    window.initWindow();
    initVulkan();
}

void Scene::initVulkan() {
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

void Scene::mainLoop() {
    while (!glfwWindowShouldClose(window.getGLFWwindow())) {
        processKeyboardInput();
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void Scene::cleanup() {
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

void Scene::processKeyboardInput() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    camera.deltaTime = currentFrame - camera.lastFrame;
    camera.lastFrame = currentFrame;
    if(glfwGetKey(window.getGLFWwindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //window->onClose();
    }
    if(glfwGetKey(window.getGLFWwindow(), GLFW_KEY_W)) {
        camera.processKeyboard(FORWARD, camera.deltaTime);
    }
    if(glfwGetKey(window.getGLFWwindow(), GLFW_KEY_A)) {
        camera.processKeyboard(LEFT, camera.deltaTime);
    }
    if(glfwGetKey(window.getGLFWwindow(), GLFW_KEY_S)) {
        camera.processKeyboard(BACKWARD, camera.deltaTime);
    }
    if(glfwGetKey(window.getGLFWwindow(), GLFW_KEY_D)) {
        camera.processKeyboard(RIGHT, camera.deltaTime);
    }
    bool uKeyIsPressed = (glfwGetKey(window.getGLFWwindow(), GLFW_KEY_U) == GLFW_PRESS);
    if (uKeyIsPressed && !uKeyWasPressed) {
        camera.setFlashlight(!camera.isFlashlight());
    }
    uKeyWasPressed = uKeyIsPressed;
}

void Scene::processMouseInput(double xPos, double yPos) {
    camera.processMouseMovement(xPos, yPos, true);
}

void Scene::processMouseScrollInput([[maybe_unused]] double xOffset, double yOffset) {
    camera.zoomWithMouseScroll(yOffset);
}
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
    DescriptorPool::createDescriptorSetLayout();

    // GRAPHICS PIPELINE
    graphicsPipelines.insert({"default", RenderPass::createGraphicsPipeline("shaders/shader.vert.spv", "shaders/shader.frag.spv", VK_FRONT_FACE_COUNTER_CLOCKWISE)});
    graphicsPipelines.insert({"plane", RenderPass::createGraphicsPipeline("shaders/shader.vert.spv", "shaders/shader.frag.spv", VK_FRONT_FACE_CLOCKWISE)});
    graphicsPipelines.insert({"red", RenderPass::createGraphicsPipeline("shaders/shader_red.vert.spv", "shaders/shader_red.frag.spv", VK_FRONT_FACE_COUNTER_CLOCKWISE)});
    graphicsPipelines.insert({"skybox", RenderPass::createGraphicsPipelineSkybox("shaders/shader_skybox.vert.spv", "shaders/shader_skybox.frag.spv")});

    CommandPool::createCommandPool();
    SwapChain::createDepthResources();
    SwapChain::createFramebuffers();

    meshes.insert({"viking_room", new Mesh{"assets/models/viking_room.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"cube", new Mesh{"assets/models/cube.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"covered_car", new Mesh{"assets/car/model/covered_car_1k.obj", "assets/car/textures/covered_car_diff_1k.jpg"}});
    meshes.insert({"skybox", new Mesh{"assets/skybox/model/skybox.obj", texturePaths}});
    meshes.insert({"plane", new Mesh{10.0f, 10.0f, "assets/terrain/leafy_grass_diff_1k.jpg"}});

    // TEXTURE
    for (auto& mesh : meshes) {
        if(mesh.second->isCubeMap) {
            mesh.second->createCubeMapTextures();
        } else {
            mesh.second->createTextures();
        }
    }

    DescriptorPool::createDescriptorPool(meshes.size());
    // Hier werden alle unsere Objekte geladen die wir in der Szene brauchen
    // Pfade zu der obj sind gerade noch hard coded
    
    for (auto& mesh : meshes) {
        mesh.second->initBuffers();
    }

    CommandPool::createCommandBuffers();
    createSyncObjects();

    initSceneGraph();
}

void Scene::initSceneGraph() {
    rootNode = new SceneNode(nullptr, "default");

    auto meshNode1 = new SceneNode(meshes["viking_room"], "default");
    auto meshNode2 = new SceneNode(meshes["cube"], "red");
    auto meshNode3 = new SceneNode(meshes["covered_car"], "default");
    auto skybox = new SceneNode(meshes["skybox"], "skybox");
    auto plane = new SceneNode(meshes["plane"], "plane");

    rootNode->addChild(skybox);
    rootNode->addChild(plane);
    rootNode->addChild(meshNode1);
    rootNode->addChild(meshNode2);
    meshNode2->addChild(meshNode3);

    glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    
    meshNode1->setLocalTransform(transform1);
    meshNode2->setLocalTransform(transform2);
    meshNode3->setLocalTransform(transform1);
    skybox->setLocalTransform(glm::mat4(1.0f));
    plane->setLocalTransform(glm::mat4(1.0f));
}

void Scene::waitOutstandingQueues() {
    // To wait on the host for the completion of outstanding queue operations for all queues on a given logical device
    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void Scene::cleanup() {
    SwapChain::cleanupSwapChain();

    for (auto& graphicsPipeline : graphicsPipelines) {
        vkDestroyPipeline(LogicalDeviceWrapper::getVkDevice(), graphicsPipeline.second.graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(LogicalDeviceWrapper::getVkDevice(), graphicsPipeline.second.pipelineLayout, nullptr);
    }

    vkDestroyRenderPass(LogicalDeviceWrapper::getVkDevice(), RenderPass::getRenderPass(), nullptr);

    vkDestroyDescriptorPool(LogicalDeviceWrapper::getVkDevice(), DescriptorPool::getDescriptorPool(), nullptr);

    for (auto& mesh : meshes) {
        mesh.second->cleanupTextures();
    }    

    vkDestroyDescriptorSetLayout(LogicalDeviceWrapper::getVkDevice(), DescriptorPool::getDescriptorSetLayout(), nullptr);
    
    for (auto& mesh : meshes) {
        delete mesh.second;
    }  

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

void Scene::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (ssize_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(LogicalDeviceWrapper::getVkDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(LogicalDeviceWrapper::getVkDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(LogicalDeviceWrapper::getVkDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Scene::drawFrame() {
    vkWaitForFences(LogicalDeviceWrapper::getVkDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(LogicalDeviceWrapper::getVkDevice(), SwapChain::getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        SwapChain::recreateSwapChain(window);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(LogicalDeviceWrapper::getVkDevice(), 1, &inFlightFences[currentFrame]);

    vkResetCommandBuffer(CommandPool::getCommandBuffers()[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(CommandPool::getCommandBuffers()[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass::getRenderPass();
    renderPassInfo.framebuffer = SwapChain::getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = SwapChain::getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}}; 
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(CommandPool::getCommandBuffers()[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    rootNode->updateWorldTransform();

    // First render skybox separately
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() == "skybox") {
            child->draw(CommandPool::getCommandBuffers()[currentFrame], 
                       graphicsPipelines, currentFrame, camera);
        }
    }

    // Then render other objects
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() != "skybox") {
            child->draw(CommandPool::getCommandBuffers()[currentFrame], 
                       graphicsPipelines, currentFrame, camera);
        }
    }
    
    rootNode->draw(CommandPool::getCommandBuffers()[currentFrame], graphicsPipelines, currentFrame, camera);

    vkCmdEndRenderPass(CommandPool::getCommandBuffers()[currentFrame]);

    if (vkEndCommandBuffer(CommandPool::getCommandBuffers()[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &CommandPool::getCommandBuffers()[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(LogicalDeviceWrapper::getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {SwapChain::getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(LogicalDeviceWrapper::getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized()) {
        window->setFramebufferResized(false);
        SwapChain::recreateSwapChain(window);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}
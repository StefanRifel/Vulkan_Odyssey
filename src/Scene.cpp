#include "Scene.h"

Scene::Scene(Window* window) : window(window) {
}

void Scene::initVulkan() {
    InstanceWrapper::createInstance();
    InstanceWrapper::setupDebugMessenger();
    InstanceWrapper::createSurface(window->getGLFWwindow());

    PhysicalDeviceWrapper::pickPhysicalDevice();
    LogicalDeviceWrapper::createLogicalDevice();

    renderer = new Renderer(window);
    renderSystem = new RenderSystem(renderer);

    meshes.insert({"viking_room", new Mesh{"assets/models/viking_room.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"cube", new Mesh{"assets/models/cube.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"moon", new Mesh{"assets/moon/model/moon.obj", "assets/moon/textures/brown_mud_diff_1k.jpg"}});
    meshes.insert({"skybox", new Mesh{"assets/skybox/model/skybox.obj", texturePaths}});
    meshes.insert({"plane", new Mesh{10.0f, 10.0f, "assets/terrain/leafy_grass_diff_1k.jpg"}});

    // TEXTURE
    for (auto& mesh : meshes) {
        if(mesh.second->isCubeMap) {
            mesh.second->createCubeMapTextures(renderer->getCommandPool());
        } else {
            mesh.second->createTextures(renderer->getCommandPool());
        }
    }

    renderSystem->createDescriptorPool(meshes.size());
    // Hier werden alle unsere Objekte geladen die wir in der Szene brauchen
    // Pfade zu der obj sind gerade noch hard coded
    
    for (auto& mesh : meshes) {
        mesh.second->initBuffers(renderSystem->getDescriptorPool(), renderer->getCommandPool());
    }

    initSceneGraph();
}

void Scene::initSceneGraph() {
    rootNode = new SceneNode(nullptr, "default");

    auto meshNode1 = new SceneNode(meshes["viking_room"], "default");
    auto meshNode2 = new SceneNode(meshes["cube"], "red");
    auto meshNode3 = new SceneNode(meshes["moon"], "default");
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
    // Warte bis alle Queues fertig sind
    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void Scene::cleanup() {
    delete renderer;

    delete renderSystem;

    for (auto& mesh : meshes) {
        mesh.second->cleanupTextures();
    }    
    
    for (auto& mesh : meshes) {
        delete mesh.second;
    }  

    LogicalDeviceWrapper::cleanup();
    InstanceWrapper::cleanup();
}

Camera& Scene::getCamera() {
    return camera;
}


void Scene::recordCommandBuffers(int imageIndex) {
    // Beginne das Aufzeichnen des Command Buffers
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderer->getSwapChain()->getRenderPass()->getRenderPass();
    renderPassInfo.framebuffer = renderer->getSwapChain()->getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = renderer->getSwapChain()->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}}; 
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    rootNode->updateWorldTransform();

    // First render skybox separately
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() == "skybox") {
            child->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(),
                       renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);
        }
    }

    // Then render other objects
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() != "skybox") {
            child->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(),
            renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);
        }
    }
    
    rootNode->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(), renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);

    vkCmdEndRenderPass(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame]);

    if (vkEndCommandBuffer(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Scene::drawFrame() {
    vkWaitForFences(LogicalDeviceWrapper::getVkDevice(), 1, &renderer->getSwapChain()->inFlightFences[renderer->getSwapChain()->currentFrame], VK_TRUE, UINT64_MAX);

    // Hole das nächste Bild aus der SwapChain
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(LogicalDeviceWrapper::getVkDevice(), renderer->getSwapChain()->getSwapChain(), UINT64_MAX, renderer->getSwapChain()->imageAvailableSemaphores[renderer->getSwapChain()->currentFrame], VK_NULL_HANDLE, &imageIndex);

    // Wenn die SwapChain veraltet ist, dann erstelle eine neue
    // Die SwapChain ist veraltet, wenn das Fenster vergrößert oder verkleinert wird
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        renderer->getSwapChain()->recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Überprüfe ob ein Bild in der Queue ist, das noch nicht fertig ist
    vkResetFences(LogicalDeviceWrapper::getVkDevice(), 1, &renderer->getSwapChain()->inFlightFences[renderer->getSwapChain()->currentFrame]);

    vkResetCommandBuffer(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], /*VkCommandBufferResetFlagBits*/ 0);

    recordCommandBuffers(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {renderer->getSwapChain()->imageAvailableSemaphores[renderer->getSwapChain()->currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame];

    VkSemaphore signalSemaphores[] = {renderer->getSwapChain()->renderFinishedSemaphores[renderer->getSwapChain()->currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(LogicalDeviceWrapper::getGraphicsQueue(), 1, &submitInfo, renderer->getSwapChain()->inFlightFences[renderer->getSwapChain()->currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {renderer->getSwapChain()->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(LogicalDeviceWrapper::getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized()) {
        window->resetWindowResizedFlag();
        renderer->getSwapChain()->recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    renderer->getSwapChain()->currentFrame = (renderer->getSwapChain()->currentFrame + 1) % renderer->getSwapChain()->MAX_FRAMES_IN_FLIGHT;
}
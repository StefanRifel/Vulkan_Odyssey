#include "Renderer.h"

void Renderer::createCommandBuffers() {
    commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(LogicalDeviceWrapper::getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = PhysicalDeviceWrapper::findQueueFamilies(PhysicalDeviceWrapper::getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(LogicalDeviceWrapper::getVkDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void Renderer::beginFrame() {
    vkWaitForFences(LogicalDeviceWrapper::getVkDevice(), 1, &swapChain->inFlightFences[swapChain->currentFrame], VK_TRUE, UINT64_MAX);

    // Hole das nächste Bild aus der SwapChain
    VkResult result = vkAcquireNextImageKHR(LogicalDeviceWrapper::getVkDevice(), swapChain->getSwapChain(), UINT64_MAX, swapChain->imageAvailableSemaphores[swapChain->currentFrame], VK_NULL_HANDLE, &imageIndex);

    // Wenn die SwapChain veraltet ist, dann erstelle eine neue
    // Die SwapChain ist veraltet, wenn das Fenster vergrößert oder verkleinert wird
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapChain->recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    // Überprüfe ob ein Bild in der Queue ist, das noch nicht fertig ist
    vkResetFences(LogicalDeviceWrapper::getVkDevice(), 1, &swapChain->inFlightFences[swapChain->currentFrame]);

    vkResetCommandBuffer(commandBuffers[swapChain->currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
}

void Renderer::endFrame() {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {swapChain->imageAvailableSemaphores[swapChain->currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[swapChain->currentFrame];

    VkSemaphore signalSemaphores[] = {swapChain->renderFinishedSemaphores[swapChain->currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(LogicalDeviceWrapper::getGraphicsQueue(), 1, &submitInfo, swapChain->inFlightFences[swapChain->currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain->getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    VkResult result = vkQueuePresentKHR(LogicalDeviceWrapper::getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window->getFramebufferResized()) {
        window->resetWindowResizedFlag();
        swapChain->recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    swapChain->currentFrame = (swapChain->currentFrame + 1) % swapChain->MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass() {
    // Beginne das Aufzeichnen des Command Buffers
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[swapChain->currentFrame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain->getRenderPass()->getRenderPass();
    renderPassInfo.framebuffer = swapChain->getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}}; 
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[swapChain->currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void Renderer::endSwapChainRenderPass() {
    vkCmdEndRenderPass(commandBuffers[swapChain->currentFrame]);

    if (vkEndCommandBuffer(commandBuffers[swapChain->currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
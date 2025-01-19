#include "CommandPool.h"

VkCommandPool CommandPool::commandPool = VK_NULL_HANDLE;
std::vector<VkCommandBuffer> CommandPool::commandBuffers;

VkCommandPool& CommandPool::getCommandPool() {
    return commandPool;
}

std::vector<VkCommandBuffer>& CommandPool::getCommandBuffers() {
    return commandBuffers;
}

void CommandPool::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = PhysicalDeviceWrapper::findQueueFamilies(PhysicalDeviceWrapper::getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(LogicalDeviceWrapper::getVkDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void CommandPool::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(LogicalDeviceWrapper::getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

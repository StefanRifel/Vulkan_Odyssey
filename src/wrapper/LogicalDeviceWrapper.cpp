#include "LogicalDeviceWrapper.h"

VkDevice LogicalDeviceWrapper::device = VK_NULL_HANDLE;
VkQueue LogicalDeviceWrapper::graphicsQueue = VK_NULL_HANDLE;
VkQueue LogicalDeviceWrapper::presentQueue = VK_NULL_HANDLE;

VkDevice& LogicalDeviceWrapper::getVkDevice() {
    return device;
}

VkQueue& LogicalDeviceWrapper::getGraphicsQueue() {
    return graphicsQueue;
}

VkQueue& LogicalDeviceWrapper::getPresentQueue() {
    return presentQueue;
}

void LogicalDeviceWrapper::createLogicalDevice() {
    QueueFamilyIndices indices = PhysicalDeviceWrapper::getQueueFamilyIndices();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(PhysicalDeviceWrapper::getDeviceExtensions().size());
    createInfo.ppEnabledExtensionNames = PhysicalDeviceWrapper::getDeviceExtensions().data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(InstanceWrapper::getValidationLayers().size());
        createInfo.ppEnabledLayerNames = InstanceWrapper::getValidationLayers().data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(PhysicalDeviceWrapper::getPhysicalDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void LogicalDeviceWrapper::cleanup() {
    vkDestroyDevice(device, nullptr);
}
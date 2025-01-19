#include "PhysicalDeviceWrapper.h"

SwapChainSupportDetails PhysicalDeviceWrapper::physicalDeviceSwapChainSupportDetails;
QueueFamilyIndices PhysicalDeviceWrapper::physicalDevicequeueFamilyIndices;
VkPhysicalDevice PhysicalDeviceWrapper::physicalDevice = VK_NULL_HANDLE;
const std::vector<const char*> PhysicalDeviceWrapper::deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

void PhysicalDeviceWrapper::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(InstanceWrapper::getInstance(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(InstanceWrapper::getInstance(), &deviceCount, devices.data());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        if (isDeviceSuitable(device) && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            physicalDevice = device;
            physicalDevicequeueFamilyIndices = findQueueFamilies(device);
            physicalDeviceSwapChainSupportDetails = querySwapChainSupport(device);

            VkPhysicalDeviceMemoryProperties memoryProperties;
            vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

            Logger::log("=== Vulkan Physical Device Properties ===");

            Logger::log("Device Name: ", deviceProperties.deviceName);
            Logger::log("Device ID: ", deviceProperties.deviceID);
            Logger::log("Vendor ID: ", deviceProperties.vendorID);
            Logger::log("API Version: ", 
                        VK_VERSION_MAJOR(deviceProperties.apiVersion), ".", 
                        VK_VERSION_MINOR(deviceProperties.apiVersion), ".", 
                        VK_VERSION_PATCH(deviceProperties.apiVersion));
            Logger::log("Driver Version: ", 
                        VK_VERSION_MAJOR(deviceProperties.driverVersion), ".", 
                        VK_VERSION_MINOR(deviceProperties.driverVersion), ".", 
                        VK_VERSION_PATCH(deviceProperties.driverVersion));

            Logger::log("=== Device Limits ===");
            Logger::log("Max Image Dimension 2D: ", deviceProperties.limits.maxImageDimension2D);
            Logger::log("Max Image Dimension 3D: ", deviceProperties.limits.maxImageDimension3D);
            Logger::log("Max Image Array Layers: ", deviceProperties.limits.maxImageArrayLayers);
            Logger::log("Max Memory Allocation Count: ", deviceProperties.limits.maxMemoryAllocationCount);
            Logger::log("Max Uniform Buffer Range: ", deviceProperties.limits.maxUniformBufferRange, " bytes");
            Logger::log("Max Vertex Input Attributes: ", deviceProperties.limits.maxVertexInputAttributes);
            Logger::log("Max Vertex Input Bindings: ", deviceProperties.limits.maxVertexInputBindings);

            Logger::log("=== Device Features ===");
            Logger::log("Geometry Shader Support: ", 
                        (deviceProperties.limits.maxGeometryShaderInvocations > 0 ? "Yes" : "No"));
            Logger::log("Tessellation Shader Support: ", 
                        (deviceProperties.limits.maxTessellationPatchSize > 0 ? "Yes" : "No"));

            Logger::log("========================================");
            Logger::log("=== Vulkan Physical Device Memory Properties ===");

            Logger::log("Memory Type Count: ", memoryProperties.memoryTypeCount);
            
            for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
                const VkMemoryType& memType = memoryProperties.memoryTypes[i];
                Logger::log("  Memory Type ", i, ": ");
                Logger::log("    Property Flags: ", memType.propertyFlags);
                Logger::log("    Heap Index: ", memType.heapIndex);
            }

            Logger::log("Memory Heap Count: ", memoryProperties.memoryHeapCount);
            
            for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i) {
                const VkMemoryHeap& memHeap = memoryProperties.memoryHeaps[i];
                Logger::log("  Memory Heap ", i, ": ");
                Logger::log("    Size: ", memHeap.size, " bytes (", 
                            (memHeap.size / (1024.0 * 1024.0)), " MB)");
                Logger::log("    Flags: ", memHeap.flags);
            }

            // Anzahl der verfügbaren Queue-Familien abfragen
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            // Eigenschaften der Queue-Familien abfragen
            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            Logger::log("========================================");
            Logger::log("=== Vulkan Queue Family Properties ===");

            // Queue-Familien loggen
            for (uint32_t i = 0; i < queueFamilyCount; ++i) {
                const auto& queueFamily = queueFamilies[i];

                Logger::log("  Queue-Familie ", i, ":");
                Logger::log("    Anzahl der Queues: ", queueFamily.queueCount);

                Logger::log("    Unterstützte Typen:");
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    Logger::log("      - Grafik");
                }
                if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                    Logger::log("      - Compute");
                }
                if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                    Logger::log("      - Transfer");
                }
                if (queueFamily.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
                    Logger::log("      - Sparse Binding");
                }

                // Präsentationsunterstützung prüfen
                VkBool32 presentSupport = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, InstanceWrapper::getVkSurfaceKHR(), &presentSupport);
                Logger::log("    Präsentationsunterstützung: ", presentSupport ? "Ja" : "Nein");

                // Timestamp-Validierungsbits
                Logger::log("    Timestamp-Validierung: ", queueFamily.timestampValidBits > 0 ? "Ja" : "Nein");

                // Minimale Transfer-Granularität
                Logger::log("    Min. Transfer Granularität: ", 
                            queueFamily.minImageTransferGranularity.width, "x", 
                            queueFamily.minImageTransferGranularity.height, "x", 
                            queueFamily.minImageTransferGranularity.depth);
            }

            Logger::log("========================================");
            break;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

bool PhysicalDeviceWrapper::isDeviceSuitable(VkPhysicalDevice device) {

    QueueFamilyIndices indices = PhysicalDeviceWrapper::findQueueFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

bool PhysicalDeviceWrapper::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails PhysicalDeviceWrapper::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, InstanceWrapper::getVkSurfaceKHR(), &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, InstanceWrapper::getVkSurfaceKHR(), &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, InstanceWrapper::getVkSurfaceKHR(), &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, InstanceWrapper::getVkSurfaceKHR(), &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, InstanceWrapper::getVkSurfaceKHR(), &presentModeCount, details.presentModes.data());
    }

    return details;
}

QueueFamilyIndices PhysicalDeviceWrapper::findQueueFamilies(VkPhysicalDevice device) {

    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, InstanceWrapper::getVkSurfaceKHR(), &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

/**
 * @brief Findet einen geeigneten Speichertyp basierend auf den angegebenen Anforderungen.
 * 
 * Überprüft die Speichertypen des Physical Devices und gibt den Index eines Typs zurück,
 * der sowohl durch die `typeFilter`-Maske als auch durch die gewünschten `properties` unterstützt wird.
 * 
 * @param typeFilter Bitmaske, die die zulässigen Speichertypen angibt.
 * @param properties Flags, die die benötigten Speicher-Eigenschaften definieren.
 * @return uint32_t Index des geeigneten Speichertyps.
 * @throws std::runtime_error Wenn kein passender Speichertyp gefunden wird.
 */

uint32_t PhysicalDeviceWrapper::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

VkPhysicalDevice& PhysicalDeviceWrapper::getPhysicalDevice() {
    return physicalDevice;
}

const std::vector<const char*>& PhysicalDeviceWrapper::getDeviceExtensions() {
    return deviceExtensions;
}

SwapChainSupportDetails& PhysicalDeviceWrapper::getSwapChainSupportDetails() {
    return physicalDeviceSwapChainSupportDetails;
}

QueueFamilyIndices& PhysicalDeviceWrapper::getQueueFamilyIndices() {
    return physicalDevicequeueFamilyIndices;
}
#include "InstanceWrapper.h"

VkInstance InstanceWrapper::instance = VK_NULL_HANDLE;
VkDebugUtilsMessengerEXT InstanceWrapper::debugMessenger = VK_NULL_HANDLE;
VkSurfaceKHR InstanceWrapper::surface = VK_NULL_HANDLE;

const std::vector<const char*> InstanceWrapper::validationLayers = {"VK_LAYER_KHRONOS_validation"};

VkInstance& InstanceWrapper::getInstance() {
    return instance;
}

VkSurfaceKHR& InstanceWrapper::getVkSurfaceKHR() {
    return surface;
}

VkDebugUtilsMessengerEXT& InstanceWrapper::getDebugMessenger() {
    return debugMessenger;
}

const std::vector<const char*>& InstanceWrapper::getValidationLayers() {
    return validationLayers;
}

void InstanceWrapper::createInstance() {
    // Prüft, ob Validierungsschichten benötigt werden und ob sie verfügbar sind.
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan_odyssey";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // Holt sich alle Erweiterungen, die für die Vulkan-Instanz erforderlich sind
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Wenn die Validierungsschicht aktiviert ist, fügen wir die Debug-Messenger-Erstellungsinformationen hinzu.
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void InstanceWrapper::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (CreateDebugUtilsMessengerEXT(&createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void InstanceWrapper::createSurface(GLFWwindow* window) {
    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

bool InstanceWrapper::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

// Gibt die Erweiterungen zurück, die für die Vulkan-Instanz erforderlich sind.
std::vector<const char*> InstanceWrapper::getRequiredExtensions() {
    // ----------loggt alle existierenden extentions----------
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> allAvalibleextensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allAvalibleextensions.data());

    Logger::log("Verfügbaren extensions: (" + std::to_string(extensionCount) + ")");
    for (const auto& extension : allAvalibleextensions) {
        Logger::log("\t- " + std::string(extension.extensionName));
    }
    // --------------------

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    // Holt alle Erweiterungen, die von GLFW für die Vulkan-Instanz benötigt werden.
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Wenn Validierungsschichten aktiviert sind, fügen wir die Debug-Messenger-Erweiterung hinzu.
    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // ----------loggt alle benötigten extentions----------
    Logger::log("Benötigte extensions: (" + std::to_string(glfwExtensionCount) + ")");
    for (const char* extension : extensions) {
        Logger::log("\t- " + std::string(extension));
    }
    // --------------------

    return extensions;
}

void InstanceWrapper::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VkResult InstanceWrapper::CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void InstanceWrapper::DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

void InstanceWrapper::cleanup() {
    if (enableValidationLayers) {
        InstanceWrapper::DestroyDebugUtilsMessengerEXT(nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}
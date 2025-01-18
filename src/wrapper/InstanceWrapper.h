#ifndef INSTANCEWRAPPER_H
#define INSTANCEWRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class InstanceWrapper {

private:
    /*
        In Vulkan ist ein VkInstance eine grundlegende Struktur, die die 
        Verbindung zwischen der Anwendung und der Vulkan-Implementierung herstellt. 
        Es repräsentiert den Einstiegspunkt in die Vulkan-API und dient als übergeordneter 
        Container für alle Vulkan-Objekte.
    */
    static VkInstance instance;

    static VkDebugUtilsMessengerEXT debugMessenger;
    static VkSurfaceKHR surface;

    static const std::vector<const char*> validationLayers;

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback([[maybe_unused]] VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

public:
    static VkInstance& getInstance();
    static const std::vector<const char*>& getValidationLayers();
    static VkSurfaceKHR& getVkSurfaceKHR();
    static VkDebugUtilsMessengerEXT& getDebugMessenger();

    static void createInstance();
    static void setupDebugMessenger();
    static void createSurface(GLFWwindow* window);

    static bool checkValidationLayerSupport();
    static std::vector<const char*> getRequiredExtensions();
    static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    static void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

    static void cleanup();
};

#endif
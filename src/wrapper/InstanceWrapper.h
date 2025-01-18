#ifndef INSTANCEWRAPPER_H
#define INSTANCEWRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <cstring>
#include <iostream>

#include "../util/Logger.h"

// Aktiviert Validierungsschichten nur, wenn NDEBUG nicht definiert ist (Debug-Modus).
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


/**
 * @class InstanceWrapper
 * @brief Wrapper-Klasse für die Vulkan-Instanz, den Debug-Messenger und die Fensteroberfläche.
 * 
 * Diese Klasse stellt Funktionen zum Erstellen und Verwalten einer Vulkan-Instanz,
 * sowie zum Einrichten eines Debug-Messengers und der Fensteroberfläche für Vulkan zur Verfügung.
 * Sie bietet außerdem Funktionen zum Überprüfen von Validierungsschichten.
 */
class InstanceWrapper {

private:
    // Vulkan-Instanz, die die Verbindung zur Vulkan-API darstellt.
    static VkInstance instance;

    // Vulkan-Objekt für den Debug-Messenger, der Debug-Nachrichten verwaltet.
    static VkDebugUtilsMessengerEXT debugMessenger;

    // Vulkan-Oberfläche, die vom Fenster (GLFW) für die Darstellung erstellt wird.
    static VkSurfaceKHR surface;

    // Liste der Validierungsschichten, die während der Entwicklung für Debugging-Zwecke genutzt werden.
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
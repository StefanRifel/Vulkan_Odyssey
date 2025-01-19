#ifndef PHYSICALDEVICEWRAPPER_H
#define PHYSICALDEVICEWRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../types/QueueFamilyIndices.h"
#include "../types/SwapChainSupportDetails.h"
#include "InstanceWrapper.h"

#include <vector>
#include <set>

/**
 * @brief Wrapper-Klasse für die Verwaltung eines Vulkan Physical Devices.
 * 
 * Diese Klasse abstrahiert die Auswahl, Überprüfung und Nutzung eines Physical Devices 
 * (z. B. einer GPU), das Vulkan unterstützt. Sie stellt Funktionen zur Verfügung, 
 * um die Eignung des Geräts zu prüfen, unterstützte Features und Extensions zu ermitteln 
 * sowie Speicher- und Queue-Familien zu verwalten.
 */
class PhysicalDeviceWrapper {

private:
    /*
     * `VkPhysicalDevice` repräsentiert eine verfügbare GPU oder ein anderes Gerät, 
     * das Vulkan-kompatibel ist. Dieses Handle verweist auf die ausgewählte GPU.
     */
    static VkPhysicalDevice physicalDevice; 
    
    static SwapChainSupportDetails physicalDeviceSwapChainSupportDetails;
    static QueueFamilyIndices physicalDevicequeueFamilyIndices;

    /*
     * Eine Liste von Vulkan Device-Extensions, die für die Anwendung erforderlich sind,
     * z. B. Unterstützung für Swapchains. Diese Extensions müssen vom Gerät unterstützt werden.
     */
    const static std::vector<const char*> deviceExtensions;

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    static bool isDeviceSuitable(VkPhysicalDevice device);
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    
public:
    static VkPhysicalDevice& getPhysicalDevice();
    static const std::vector<const char*>& getDeviceExtensions();
    static SwapChainSupportDetails& getSwapChainSupportDetails();
    static QueueFamilyIndices& getQueueFamilyIndices();

    static void pickPhysicalDevice();

    /*
     * @brief Findet einen geeigneten Speichertyp basierend auf den angegebenen Anforderungen.
     * 
     * Überprüft die Speichertypen des Physical Devices und gibt den Index eines Typs zurück,
     * der sowohl durch die `typeFilter`-Maske als auch durch die gewünschten `properties` unterstützt wird.
     */
    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

};

#endif
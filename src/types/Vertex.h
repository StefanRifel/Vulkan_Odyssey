#ifndef VERTEX_H
#define VERTEX_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;

    /*
    Diese Funktion definiert die Binding-Beschreibung für den Vertex-Puffer. 
    Sie beschreibt, wie die Vertex-Daten im Puffer organisiert sind und wie die GPU sie interpretieren soll.
    */  
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        // Gibt die Schrittweite an 
        bindingDescription.stride = sizeof(Vertex);

        // - VK_VERTEX_INPUT_RATE_VERTEX: Ein neues Vertex wird für jede neue Vertex-Instanz verarbeitet.
        // - VK_VERTEX_INPUT_RATE_INSTANCE: Die Daten werden einmal pro Instanz verarbeitet.
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && normal == other.normal && texCoord == other.texCoord;
    }
};

/*
 * Diese Funktion definiert eine Hash-Berechnung für den Typ Vertex. 
 * Sie wird benötigt, um Vertex-Objekte in Containern wie std::unordered_map oder std::unordered_set zu verwenden. 
 * Der Hash-Wert wird berechnet, indem die Hash-Werte der Position (pos), der Farbe (color) 
 * und der Texturkoordinaten (texCoord) kombiniert werden. 
 */
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

#endif //VERTEX_H

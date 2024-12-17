#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.h"
#include "loader/ModelLoader.h"
#include "wrapper/InstanceWrapper.h"
#include "base/SwapChain.h"
#include "Camera.h"

#include "renderable/Mesh.h"
#include <memory>
#include "SceneNode.h"

class Scene {

private:
    Window* window;
    Camera camera;

    Mesh* mesh = new Mesh{"assets/models/viking_room.obj", "assets/textures/viking_room.png"};
    Mesh* mesh2 = new Mesh{"assets/models/cube.obj", "assets/textures/viking_room.png"};
    
    SceneNode* rootNode;
    std::map<std::string ,GraphicsPipeline> graphicsPipelines;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

public:
    Scene(Window* window);

    void initVulkan();
    void waitOutstandingQueues();
    void cleanup();

    Camera& getCamera();

    void createSyncObjects();
    void drawFrame();
};

#endif
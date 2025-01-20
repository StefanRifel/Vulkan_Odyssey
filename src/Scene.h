#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.h"
#include "loader/ModelLoader.h"
#include "wrapper/InstanceWrapper.h"
#include "base/SwapChain.h"
#include "Camera.h"
#include "base/GraphicPipeline.h"

#include "renderable/Mesh.h"
#include <memory>
#include "SceneNode.h"

class Scene {

private:
    Window* window;
    Camera camera;

    std::map<std::string, Mesh*> meshes;
    
    std::vector<std::string> texturePaths = {
        "assets/skybox/textures/right.png",
        "assets/skybox/textures/left.png",
        "assets/skybox/textures/top.png",
        "assets/skybox/textures/bottom.png",
        "assets/skybox/textures/front.png",
        "assets/skybox/textures/back.png"
    };

    SceneNode* rootNode;
    std::map<std::string ,GraphicPipeline*> graphicPipelines;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

public:
    Scene(Window* window);

    void initVulkan();
    void initSceneGraph();
    void waitOutstandingQueues();
    void cleanup();

    Camera& getCamera();

    void createSyncObjects();
    void drawFrame();
    void recordCommandBuffers(int imageIndex);
};

#endif
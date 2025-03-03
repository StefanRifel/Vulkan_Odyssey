#ifndef SCENE_H
#define SCENE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Camera.h"
#include "base/Renderer.h"
#include "system/RenderSystem.h"

class Scene {

private:
    Window* window;
    Camera camera;

    Renderer* renderer;
    RenderSystem* renderSystem;


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

public:
    Scene(Window* window);

    void initVulkan();
    void initSceneGraph();
    void waitOutstandingQueues();
    void cleanup();

    Camera& getCamera();

    void drawFrame();
};

#endif
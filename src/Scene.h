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
#include "system/RenderSystem.h"
#include "base/Renderer.h"

#include "renderable/Mesh.h"
#include <memory>
#include "SceneNode.h"

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
    void recordCommandBuffers(int imageIndex);
};

#endif
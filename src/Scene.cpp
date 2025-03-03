#include "Scene.h"

Scene::Scene(Window* window) : window(window) {
}

void Scene::initVulkan() {
    InstanceWrapper::createInstance();
    InstanceWrapper::setupDebugMessenger();
    InstanceWrapper::createSurface(window->getGLFWwindow());

    PhysicalDeviceWrapper::pickPhysicalDevice();
    LogicalDeviceWrapper::createLogicalDevice();

    renderer = new Renderer(window);
    renderSystem = new RenderSystem(renderer);

    meshes.insert({"viking_room", new Mesh{"assets/models/viking_room.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"cube", new Mesh{"assets/models/cube.obj", "assets/textures/viking_room.png"}});
    meshes.insert({"moon", new Mesh{"assets/moon/model/moon.obj", "assets/moon/textures/brown_mud_diff_1k.jpg"}});
    meshes.insert({"skybox", new Mesh{"assets/skybox/model/skybox.obj", texturePaths}});
    meshes.insert({"plane", new Mesh{100.0f, 100.0f, "assets/terrain/leafy_grass_diff_1k.jpg"}});

    // TEXTURE
    for (auto& mesh : meshes) {
        if(mesh.second->isCubeMap) {
            mesh.second->createCubeMapTextures(renderer->getCommandPool());
        } else {
            mesh.second->createTextures(renderer->getCommandPool());
        }
    }

    renderSystem->createDescriptorPool(meshes.size());
    // Hier werden alle unsere Objekte geladen die wir in der Szene brauchen
    // Pfade zu der obj sind gerade noch hard coded
    
    for (auto& mesh : meshes) {
        mesh.second->initBuffers(renderSystem->getDescriptorPool(), renderer->getCommandPool());
    }

    initSceneGraph();
}

void Scene::initSceneGraph() {
    rootNode = new SceneNode(nullptr, "default");
    auto car = new SceneNode(meshes["car"], "default");
    auto tree = new SceneNode(meshes["tree"], "default");
    auto moon = new SceneNode(meshes["moon"], "default");
    auto fern= new SceneNode(meshes["fern"], "default");
    auto nettlePlant = new SceneNode(meshes["nettle_plant"], "default");
    auto picnicTable = new SceneNode(meshes["picnic_table"], "default");
    auto treeStump = new SceneNode(meshes["tree_stump"], "default");
    auto rat = new SceneNode(meshes["rat"], "default");
    auto skybox = new SceneNode(meshes["skybox"], "skybox");
    auto plane = new SceneNode(meshes["plane"], "plane");

    rootNode->addChild(skybox);
    rootNode->addChild(plane);
    rootNode->addChild(car);
    rootNode->addChild(tree);
    rootNode->addChild(moon);
    rootNode->addChild(fern);
    rootNode->addChild(nettlePlant);
    rootNode->addChild(picnicTable);
    rootNode->addChild(treeStump);
    rootNode->addChild(rat);

    glm::mat4 transform1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    glm::mat4 transform3 = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
    glm::mat4 transform4 = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
    
    glm::mat4 transformCar = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0, 0.2, 2.0));
    glm::mat4 transformRat = glm::translate(glm::mat4(1.0f), glm::vec3(-7.0, 1.6, 2.0));
    glm::mat4 transformTable = glm::translate(glm::mat4(1.0f), glm::vec3(4.0, 0.3, 0.0));
    glm::mat4 transformChessBoard = glm::translate(glm::mat4(1.0f), glm::vec3(4.0, 1.04, 0.0));
    glm::mat4 transformTree = glm::translate(glm::mat4(1.0f), glm::vec3(4.0, 0.3, 10.0));
  
    
    car->setLocalTransform(transformCar);
    tree->setLocalTransform(transformTree);
    moon->setLocalTransform(transform1);
    fern->setLocalTransform(transform3);
    nettlePlant->setLocalTransform(transform4);
    picnicTable->setLocalTransform(transformTable);
    treeStump->setLocalTransform(transform2);
    rat->setLocalTransform(transformRat);
    skybox->setLocalTransform(glm::mat4(1.0f));
    plane->setLocalTransform(glm::mat4(1.0f));
}

void Scene::waitOutstandingQueues() {
    // Warte bis alle Queues fertig sind
    vkDeviceWaitIdle(LogicalDeviceWrapper::getVkDevice());
}

void Scene::cleanup() {
    delete renderer;

    delete renderSystem;

    for (auto& mesh : meshes) {
        mesh.second->cleanupTextures();
    }    
    
    for (auto& mesh : meshes) {
        delete mesh.second;
    }  

    LogicalDeviceWrapper::cleanup();
    InstanceWrapper::cleanup();
}

Camera& Scene::getCamera() {
    return camera;
}

void Scene::drawFrame() {
    renderer->beginFrame();
    renderer->beginSwapChainRenderPass();

    rootNode->updateWorldTransform();

    // First render skybox separately
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() == "skybox") {
            child->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(),
                       renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);
        }
    }

    // Then render other objects
    for (auto child : rootNode->getChildren()) {
        if (child->getShaderType() != "skybox") {
            child->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(),
            renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);
        }
    }
    
    rootNode->draw(renderer->getCommandBuffers()[renderer->getSwapChain()->currentFrame], renderer->getSwapChain().get(), renderSystem->getGraphicPipelines(), renderer->getSwapChain()->currentFrame, camera);
    
    renderer->endSwapChainRenderPass();
    renderer->endFrame();
}
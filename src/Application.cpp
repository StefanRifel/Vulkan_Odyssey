#include "Application.h"

Application::Application() {
    init();
}

void Application::init() {
    window = std::make_unique<Window>(TITLE, WIDTH, HEIGHT);
    scene = std::make_unique<Scene>(window.get());
    inputManager = std::make_unique<InputManager>(window.get());

    scene->initVulkan();
}

void Application::run() {
    while(!window->shouldClose()){
        processInput();
        scene->drawFrame();
        window->pollEvents();
    }

    scene->waitOutstandingQueues();
}

void Application::processInput() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    scene->getCamera().deltaTime = currentFrame - scene->getCamera().lastFrame;
    scene->getCamera().lastFrame = currentFrame;

    if (inputManager->isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window->getGLFWwindow(), true);
    }
    if(inputManager->isKeyPressed(GLFW_KEY_W)) {
        scene->getCamera().processKeyboard(FORWARD, scene->getCamera().deltaTime);
    }
    if(glfwGetKey(window->getGLFWwindow(), GLFW_KEY_A)) {
        scene->getCamera().processKeyboard(LEFT, scene->getCamera().deltaTime);
    }
    if(glfwGetKey(window->getGLFWwindow(), GLFW_KEY_S)) {
        scene->getCamera().processKeyboard(BACKWARD, scene->getCamera().deltaTime);
    }
    if(glfwGetKey(window->getGLFWwindow(), GLFW_KEY_D)) {
        scene->getCamera().processKeyboard(RIGHT, scene->getCamera().deltaTime);
    }

    auto mousePos = inputManager->getMousePosition();
    scene->getCamera().processMouseMovement(mousePos.x, mousePos.y, true);

    auto scrollOffset = inputManager->getMouseScrollOffset();
    if (scrollOffset.x != 0.0f || scrollOffset.y != 0.0f) {
        scene->getCamera().zoomWithMouseScroll(scrollOffset.y);
    }
}

Application::~Application() {
    scene->cleanup();
}
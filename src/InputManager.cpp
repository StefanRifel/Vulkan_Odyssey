#include "InputManager.h"

InputManager::InputManager(Window* window) : window(window) {
    glfwSetWindowUserPointer(window->getGLFWwindow(), this);

    glfwSetKeyCallback(window->getGLFWwindow(), keyCallback);
    glfwSetMouseButtonCallback(window->getGLFWwindow(), mouseButtonCallback);
    glfwSetCursorPosCallback(window->getGLFWwindow(), cursorPositionCallback);
    glfwSetScrollCallback(window->getGLFWwindow(), scrollCallback);
}

InputManager::~InputManager() {
    glfwSetWindowUserPointer(window->getGLFWwindow(), nullptr);
}

bool InputManager::isKeyPressed(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool InputManager::isMouseButtonPressed(int button) const {
    auto it = mouseButtonStates.find(button);
    return it != mouseButtonStates.end() && it->second;
}

glm::vec2 InputManager::getMousePosition() const {
    return mousePosition;
}

glm::vec2 InputManager::getMouseScrollOffset() {
    glm::vec2 offset = mouseScrollOffset;
    mouseScrollOffset = glm::vec2(0.0f, 0.0f);
    return offset;
}

void InputManager::keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            inputManager->keyStates[key] = true;
        } else if (action == GLFW_RELEASE) {
            inputManager->keyStates[key] = false;
        }
    }
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        if (action == GLFW_PRESS) {
            inputManager->mouseButtonStates[button] = true;
        } else if (action == GLFW_RELEASE) {
            inputManager->mouseButtonStates[button] = false;
        }
    }
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        inputManager->mousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

void InputManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        inputManager->mouseScrollOffset += glm::vec2(static_cast<float>(xoffset), static_cast<float>(yoffset));
    }
}
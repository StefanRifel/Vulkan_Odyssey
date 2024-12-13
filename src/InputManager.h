#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include <iostream>
#include <unordered_map>

#include "Window.h"

class InputManager {
private:
    Window* window;

    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> mouseButtonStates;
    glm::vec2 mousePosition;
    glm::vec2 mouseScrollOffset;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    explicit InputManager(Window* window);
    ~InputManager();

    bool isKeyPressed(int key) const;
    bool isMouseButtonPressed(int button) const;
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseScrollOffset();
};

#endif
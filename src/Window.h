#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

class Window {

private:
    GLFWwindow* window;
    bool framebufferResized = false;
    const uint32_t WIDTH = 1024.0f;
    const uint32_t HEIGHT = 768.0f;
    bool isRunning;

public:
    GLFWwindow* getGLFWwindow();
    bool getFramebufferResized();
    void setFramebufferResized(bool b);

    void initWindow();
    void cleanup();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    static void on_window_close_callback(GLFWwindow* window);
    static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
    static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

    void onClose();
    void onMouseMovement(double xPos, double yPos) const;
    void onMouseScroll(double xOffset, double yOffset) const;
};

#endif
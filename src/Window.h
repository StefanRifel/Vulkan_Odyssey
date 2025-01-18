#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>
#include <iostream>

/**
 * @class Window
 * @brief Eine Klasse zur Verwaltung eines Fensters mit GLFW und Vulkan.
 */
class Window {

private:
    GLFWwindow* glfwWindow;

    const std::string windowName;
    const uint32_t width;
    const uint32_t height;

    bool framebufferResized = false;

    void initWindow();
public:
    Window(const std::string& windowName, int width, int height);
    ~Window();

    GLFWwindow* getGLFWwindow();

    bool shouldClose() const;
    void pollEvents() const;

    bool getFramebufferResized();
    void setFramebufferResized(bool b);

    static void framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);
};

#endif
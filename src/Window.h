#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

class Window {

private:
    GLFWwindow* glfwWindow;

    std::string title;
    uint32_t width;
    uint32_t height;

    bool framebufferResized = false;
    

public:
bool isRunning = false;
    Window(const std::string& title, int width, int height);
    ~Window();

    GLFWwindow* getGLFWwindow();
    
    void initWindow();

    bool shouldClose() const;
    void pollEvents() const;

    bool getFramebufferResized();
    void setFramebufferResized(bool b);

    static void framebufferResizeCallback(GLFWwindow* glfwWindow, int width, int height);
};

#endif
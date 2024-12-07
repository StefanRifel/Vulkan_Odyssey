#ifndef WINDOW_H
#define WINDOW_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window {

private:
    GLFWwindow* window;
    bool framebufferResized = false;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

public:
    GLFWwindow* getGLFWwindow();
    bool getFramebufferResized();
    void setFramebufferResized(bool b);

    void initWindow();
    void cleanup();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};

#endif
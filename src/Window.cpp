#include "Window.h"

GLFWwindow* Window::getGLFWwindow() {
    return window;
}

bool Window::getFramebufferResized() {
    return framebufferResized;
}

void Window::setFramebufferResized(bool b) {
    framebufferResized = b;
}

void Window::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::cleanup() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->setFramebufferResized(true);
}


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
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

    glfwSetWindowCloseCallback(window, on_window_close_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}

void Window::cleanup() {
    glfwDestroyWindow(window);

    glfwTerminate();
}

void Window::framebufferResizeCallback(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height) {
    auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    app->setFramebufferResized(true);
}

void Window::on_window_close_callback(GLFWwindow* window) {
    auto pWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    pWindow->onClose();
}

void Window::mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    auto pWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    pWindow->onMouseMovement(xPos, yPos);
}

void Window::scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    auto pWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    pWindow->onMouseScroll(xOffset, yOffset);
}

void Window::onClose() {
    isRunning = false;
}

void Window::onMouseMovement([[maybe_unused]] double xPos, [[maybe_unused]] double yPos) const {
    //scene->processMouseInput(xPos, yPos);
}

void Window::onMouseScroll([[maybe_unused]] double xOffset, [[maybe_unused]] double yOffset) const {
    //scene->processMouseScrollInput(xOffset, yOffset);
}

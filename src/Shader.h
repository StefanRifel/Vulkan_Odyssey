#ifndef SHADER_H
#define SHADER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <stdexcept>

class Shader {

private:

public:
    static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice& device);
};

#endif

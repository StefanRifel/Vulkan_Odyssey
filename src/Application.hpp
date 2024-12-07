#ifndef APPLICATION_H
#define APPLICATION_H

#include "VulkanContext.hpp"

class Application {

private:
    VulkanContext vulkanContext;
public:
    void run();
};

#endif
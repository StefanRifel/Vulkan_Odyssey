#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>

class Logger {
public:
    template <typename... Args>
    static void log(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args)); 
        std::cout << oss.str() << std::endl;
    }
};

#endif
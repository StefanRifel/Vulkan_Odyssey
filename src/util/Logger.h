#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

class Logger {
public:
    static void log(const std::string& message);
};

#endif
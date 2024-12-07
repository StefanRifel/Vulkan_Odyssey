#ifndef FILELOADER_H
#define FILELOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

inline std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file with filename: " + filename);
    }

    ssize_t fileSize = (ssize_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

#endif

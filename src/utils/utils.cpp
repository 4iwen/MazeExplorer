#include "utils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string Utils::read_entire_file(const std::string &path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

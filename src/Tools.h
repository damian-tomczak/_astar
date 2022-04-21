#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

std::vector<char> inline readFile(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::ate);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}
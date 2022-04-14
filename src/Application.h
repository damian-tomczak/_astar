#pragma once

#include "Tools.h"

#include <iostream>
#include <vector>

class Application final
{
public:
    Application(int argc, char** argv);
    ~Application();
    Application(const Application& other) = delete;
    Application& operator=(Application& other) = delete;
    Application(const Application&& other) = delete;
    Application& operator=(Application&& other) = delete;

    void run();
private:
    void loadArguments(int argc, char** argv);
    void loadGrid();
    void resizeGridSize(const char* text);
    void printGrid();

    std::string mGeneratedFile;
    std::vector<std::vector<char>> mGrid;
    uint32_t mGridWidth;
    uint32_t mGridHeight;
};

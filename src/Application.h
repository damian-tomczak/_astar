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
    void printGrid(std::string&&);
    bool astar();

    struct Astar
    {
        enum class Type
        {
            empty = 0,
            path = 3,
            obstacle = 5
        };

        struct Tile
        {
            Type type;
            uint16_t y;
            uint16_t x;
        };

        std::vector<Tile> openList;
        std::vector<Tile> closedList;
    };

    std::string mGeneratedFile;
    std::vector<std::vector<Astar::Tile>> mGrid;
    uint16_t mGridWidth;
    uint16_t mGridHeight;
};

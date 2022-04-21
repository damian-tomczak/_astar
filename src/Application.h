#pragma once

#include "Tools.h"

#include <iostream>
#include <algorithm>
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
    void printGrid(std::string&& text);
    bool astar();

    struct Astar final
    {
    public:
        Astar() = default;

        enum class Type
        {
            empty = 0,
            path = 3,
            obstacle = 5,
            start = 6,
            finish = 7
        };

        struct Tile final
        {
        public:
            Tile() : mType(Type::empty),
                y(0),
                x(0),
                mpParent(nullptr),
                mfCost(0)
            {

            }

            Type mType;
            uint16_t y;
            uint16_t x;
            Tile* mpParent;
            double mfCost;
        };

        std::vector<Tile*> mOpenList;
        std::vector<Tile*> mClosedList;

        [[nodiscard]] double calculategCost(const Tile& rTile) const
        {
            int result = 0;
            Tile* tmpParent = rTile.mpParent;
            while (tmpParent)
            {
                result++;
                tmpParent = tmpParent->mpParent;
            }

            return result;
        }

        [[nodiscard]] double calculatehCost(const Tile& rTile, const Tile& rFinish) const
        {
            return sqrt(pow(rTile.x - rFinish.x, 2) + pow(rTile.y - rFinish.y, 2));
        }

        void calculatefCost(Tile& rTile, Tile& rFinish)
        {
            double gCost = calculategCost(rTile);
            double hCost = calculatehCost(rTile, rFinish);
            rTile.mfCost = gCost + hCost;
        }
    };

    std::unique_ptr<Astar> mAstar;

    using Grid = std::vector<std::vector<Astar::Tile>>;

    Grid mGrid;
    uint16_t mGridWidth;
    uint16_t mGridHeight;
    std::string mInputFilename;
    Astar::Tile* mStart;
    Astar::Tile* mFinish;


    struct NeighborPointer final
    {
    private:
        Astar::Tile** p;
        size_t n;
    public:
        NeighborPointer(const NeighborPointer& other)
        {
            n = other.n;
            p = new Astar::Tile*[n];
            std::copy(other.begin(), other.end(), p);
        }

        NeighborPointer(size_t n) : p(nullptr), n(n)
        {
            p = new Astar::Tile*[n];

            if (p == nullptr)
            {
                throw std::runtime_error("Failed to alloc neighbors!");
            }
        }
        ~NeighborPointer()
        {
            delete[] p;
        }
        Astar::Tile*& operator[](int i)
        {
            if (i >= 0 && i <= n)
            {
                return p[i];
            }
            else
            {
                throw std::runtime_error("Selected wrong index!");
            }
        }

        Astar::Tile** begin() const { return p; }
        Astar::Tile** end() const { return p + n; }
    };

    NeighborPointer getNeighbors(const Astar::Tile& tile, uint16_t* numberOfNeighbors = nullptr)
    {
        uint16_t currentNumberOfNeighbors = 1;

        if (tile.y != 0)
        {
            currentNumberOfNeighbors++;
        }
        if (tile.y != mGridHeight - 1)
        {
            currentNumberOfNeighbors++;
        }
        if (tile.x != 0)
        {
            currentNumberOfNeighbors++;
        }
        if (tile.x != mGridWidth - 1)
        {
            currentNumberOfNeighbors++;
        }

        if (numberOfNeighbors != nullptr)
        {
            *numberOfNeighbors = currentNumberOfNeighbors;
        }

        NeighborPointer result = NeighborPointer(currentNumberOfNeighbors - 1);

        uint16_t currentItem = 0;
        if (tile.y != 0)
        {
            Astar::Tile* tmp = result[currentItem];
            result[currentItem++] = &mGrid[tile.y - 1][tile.x];
        }
        if (tile.y != mGridHeight - 1)
        {
            result[currentItem++] = &mGrid[tile.y + 1][tile.x];
        }
        if (tile.x != 0)
        {
            result[currentItem++] = &mGrid[tile.y][tile.x - 1];
        }
        if (tile.x != mGridWidth - 1)
        {
            result[currentItem++] = &mGrid[tile.y][tile.x + 1];
        }

        return result;
    }
};

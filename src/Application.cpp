#include "Application.h"

Application::Application(int argc, char** argv) : mGeneratedFile(""),
    mGrid(),
    mGridWidth(0),
    mGridHeight(0)
{
    loadArguments(argc, argv);
}

Application::~Application()
{

}

void Application::run()
{
    loadGrid();

    printGrid("The following grid has been loaded:");
    if (!astar())
    {
        throw std::runtime_error("Finding the path has failed!");
    }
    printGrid("The result of astar algorithm:");
}

void Application::loadArguments(int argc, char** argv)
{
    switch (argc)
    {
    case 1:
        throw std::runtime_error("File has not been specified.");
        break;
    case 2:
        if (strlen(argv[0]) == 0)
        {
            throw std::runtime_error("Something is wrong with the fileName!");
        }
        mGeneratedFile = argv[1];
#ifdef _DEBUG
        mGeneratedFile = "C:\\Users\\Damian\\Desktop\\astar\\grid.txt"; // TODO: debug code
#endif
        break;
    default:
        throw std::runtime_error("Expected only one argument to be passed to the program!");
        break;
    }
}

void Application::loadGrid()
{
    std::vector<char> text = readFile(mGeneratedFile);

    resizeGridSize(text.data());

    for (uint16_t i = 0, y = 0; y < mGridHeight; y++)
    {
        for (uint16_t x = 0; x < mGridWidth; x++, i += 2)
        {
            mGrid[y][x].type = static_cast<Astar::Type>(text[i] - '0');
            mGrid[y][x].x = x;
            mGrid[y][x].y = y;
        }
    }
}

void Application::resizeGridSize(const char* text)
{
    // Set width size
    for (const char* tmpText = text; tmpText && *tmpText; tmpText++)
    {
        if (*tmpText == '\n')
        {
            break;
        }
        else if (isdigit(*tmpText))
        {
            mGridWidth++;
        }
    }

    // Set height size
    uint16_t tmpWidth = 0;
    for (const char* tmpText = text;; tmpText++)
    {
        if (!(tmpText && *tmpText))
        {
            mGridHeight++;
            break;
        }
        else
        {
            if (tmpWidth > mGridWidth)
            {
                throw std::runtime_error("The horizontal length of grid size is not the same!");
            }
            else if (*tmpText == '\n')
            {
                tmpWidth = 0;
                mGridHeight++;
            }
            else if (isdigit(*tmpText))
            {
                tmpWidth++;
            }
        }
    }

    if (mGridWidth < 10 || mGridHeight < 10)
    {
        throw std::runtime_error("The grid size should be bigger!");
    }

    // Set grid size
    mGrid.resize(mGridHeight, std::vector<Astar::Tile>(mGridWidth));
}

void Application::printGrid(std::string&& text)
{
    std::cout << "\n" << text << "\n";
    for (uint16_t y = 0; y < mGridHeight; y++)
    {
        for (uint16_t x = 0; x < mGridWidth; x++)
        {
            int currentType = static_cast<int>(mGrid[y][x].type);

            std::string prefix;
            switch (currentType)
            {
            case 0:
                prefix = "\033[1;97;42m";
                break;
            case 3:
                prefix = "\033[1;97;43m";;
                break;
            case 5:
                prefix = "\033[1;97;41m";;
                break;
            }
            std::string suffix = "\033[0m";
            std::cout << prefix << currentType << suffix;
        }

        std::cout << "\n";
    }
}

bool Application::astar()
{
    using Tile = Astar::Tile;
    Tile& start = mGrid[0][0];
    Tile& finish = mGrid[mGridHeight - 1][mGridWidth - 1];



    return true;
}
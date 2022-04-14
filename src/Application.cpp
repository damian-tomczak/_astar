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
#ifdef _DEBUG
    printGrid();
#endif
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

    for (uint32_t i = 0, y = 0; y < mGridHeight; y++)
    {
        for (uint32_t x = 0; x < mGridWidth; x++, i += 2)
        {
            mGrid[y][x] = text[i];
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
    uint32_t tmpWidth = 0;
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
    mGrid.resize(mGridHeight, std::vector<char>(mGridWidth));
}

void Application::printGrid()
{
    std::cout << "The following grid has been loaded: \n";
    for (uint32_t y = 0; y < mGridHeight; y++)
    {
        for (uint32_t x = 0; x < mGridWidth; x++)
        {
            std::cout << mGrid[y][x];
            if (x != mGridWidth - 1)
            {
                std::cout << " ";
            }
        }

        std::cout << "\n";
    }
}
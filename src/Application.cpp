#include "Application.h"

Application::Application(int argc, char** argv) : mInputFilename(""),
    mGrid(),
    mGridWidth(0),
    mGridHeight(0),
    mAstar(nullptr)
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
        mInputFilename = argv[1];
#ifdef _DEBUG
        mInputFilename = "../src/grid.debug"; // TODO: debug code
#endif //_DEBUG
        break;
    default:
        throw std::runtime_error("Expected only one argument to be passed to the program!");
        break;
    }
}

void Application::loadGrid()
{
    std::vector<char> text = readFile(mInputFilename);

    resizeGridSize(text.data());

    int i = 0;
    for (int y = mGridHeight - 1; y >= 0; y--)
    {
        for (int x = 0; x < mGridWidth; x++, i += 2)
        {
            mGrid[y][x].mType = static_cast<Astar::Type>(text[i] - '0');
            mGrid[y][x].y = y;
            mGrid[y][x].x = x;
        }
    }

#ifndef _DEBUG
    mGrid[0][0].mType = Astar::Type::start;
    mGrid[mGridHeight - 1][mGridWidth - 1].mType = Astar::Type::finish;
#endif // !_DEBUG

    for (int y = 0; y < mGridHeight; y++)
    {
        for (int x = 0; x < mGridWidth; x++)
        {
            if (mGrid[x][y].mType == Astar::Type::start)
            {
                mStart = &mGrid[x][y];
            }
            else if (mGrid[x][y].mType == Astar::Type::finish)
            {
                mFinish = &mGrid[x][y];
            }
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

    if (mGridWidth < 5 || mGridHeight < 5)
    {
        throw std::runtime_error("The grid size should be bigger!");
    }

    // Set grid size
    mGrid.resize(mGridHeight, std::vector<Astar::Tile>(mGridWidth));
}

void Application::printGrid(std::string&& text)
{
    std::cout << "\n" << text << "\n";
    for (int y = mGridHeight - 1; y >= 0; y--)
    {
        for (int x = 0; x < mGridWidth; x++)
        {
            Astar::Tile* current = &mGrid[y][x];

            int currentType = static_cast<int>(current->mType);
            std::string prefix;

            switch (currentType)
            {
            case 0:
                prefix = "\033[1;97;42m";
                break;
            case 3:
                prefix = "\033[1;97;43m";
                break;
            case 5:
                prefix = "\033[1;97;41m";
                break;
            case 6:
                prefix = "\033[1;97;44m";
                break;
            case 7:
                prefix = "\033[1;97;44m";
                break;
            }

            std::cout << prefix;
            if (currentType == static_cast<int>(Astar::Type::start))
            {
                std::cout << 'S';
            }
            else if (currentType == static_cast<int>(Astar::Type::finish))
            {
                std::cout << 'F';
            }
            else
            {
                std::cout << currentType;
            }
            std::cout << "\033[0m";
        }

        std::cout << "\n";
    }
}

bool Application::astar()
{
    using Tile = Astar::Tile;
    using Type = Astar::Type;
    mAstar = std::make_unique<Astar>();
#define OPEN_LIST mAstar->mOpenList
#define CLOSED_LIST mAstar->mClosedList

    OPEN_LIST.push_back(mStart);

    while (true)
    {
        Tile* lowest = mAstar->findLowestfCostInOpen();
        CLOSED_LIST.push_back(lowest);
        OPEN_LIST.erase(std::remove_if(OPEN_LIST.begin(), OPEN_LIST.end(), [lowest](Tile* t) -> bool {
            return t == lowest ? true : false;
            }), OPEN_LIST.end());

        NeighborPointer neighbors = getNeighbors(lowest);
        for (auto current : neighbors)
        {
            if ((current->mType == Type::empty) &&
                (std::find(OPEN_LIST.begin(), OPEN_LIST.end(), current) == OPEN_LIST.end()))
            {
                OPEN_LIST.push_back(current);
                current->mpParent = lowest;
                mAstar->calculatefCost(current, mFinish);
            }
        }

#ifdef _DEBUG
        std::cout << "////////////////////////////////////////////////////////////////////////\n";
        std::cout << "Lowest:\n" << *lowest << "\n";
        std::cout << "Lowest's neighbors:\n" << neighbors << "\n";
        std::cout << "OpenList:\n";
        std::for_each(OPEN_LIST.begin(), OPEN_LIST.end(), [](Tile* tile) -> void {
            std::cout << "{ " << *tile << " }\n";
            });
        std::cout << "ClosedList:\n";
        std::for_each(CLOSED_LIST.begin(), CLOSED_LIST.end(), [](Tile* tile) -> void {
            std::cout << "{ " << *tile << " }\n";;
            });
#endif // _DEBUG
    }

    return true;
}
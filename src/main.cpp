#include "Application.h"
#include <cassert>

int main(int argc, char** argv)
{
    Application app(argc, argv);

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        assert(false);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
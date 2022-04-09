#pragma once

#include <iostream>

class Application final
{
public:
    Application();
    ~Application();
    Application(const Application& other) = delete;
    Application& operator=(Application& other) = delete;

    void run();
};

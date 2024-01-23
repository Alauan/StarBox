#include "include/main.hpp"

int main()
{
    Core *core = Core::getInstance();
    core->init();
    while (core->isRunning())
    {
        core->createFrame();
    }
    core->close();
}
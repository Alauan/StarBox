#pragma once
#include"main.hpp"


class Core
{
public:
    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    static Core *getInstance();
    void init();
    void createFrame();
    void close();
    bool isRunning();

private:
    Core();
    ~Core();

    void initSDL();

    static Core *instance;
    InputHandler *input_handler;
    PlanetManager *planet_manager;
    std::vector<Creator *> creators;
    CursorMode *cursor_mode;

    Painter *painter;
    Camera *camera;
    Timer *timer;
    SDL_Window *window;
    SDL_Renderer *renderer;

    bool running;
};
#pragma once
#include "main.hpp"


class CursorMode
{
public:
    

private:
    std::vector<SDL_Texture *> textures;
    InputHandler *input_handler;
    std::vector<Creator *> creators;
    PlanetManager *planet_manager;
    int current_brush;
    SDL_Renderer *renderer;
    Painter *painter;

    SDL_Texture *loadTexture(const char *filename, double resize_factor = 1);
public:
    CursorMode(std::vector<Creator *> creators, PlanetManager *planet_manager, SDL_Renderer *renderer);
    ~CursorMode();

    void update();
    void render();
};
#pragma once
#include "main.hpp"


class Painter
{
public:
    struct HorizontalLine
    {
        int x1;
        int x2;
        int y;
    };

    Painter(const Painter&) = delete;
    Painter& operator=(const Painter&) = delete;

    static Painter *getInstance(SDL_Window *window, SDL_Renderer *renderer, Camera *camera);
    static Painter *getInstance();

    void clearScreen();

    void drawCircle(SDL_Point center, int radius, SDL_Color color);
    void drawFilledCircle(SDL_Point center, int radius, SDL_Color color);
    void drawLine(SDL_Point start, SDL_Point end, SDL_Color color);

    void drawTraceInScreen(const std::vector<SDL_Point>& trace_on_screen, const SDL_Color& color);
    void drawTraceInWorld(const std::vector<Vector2>& trace_on_world, const SDL_Color& color);
    void drawTraceMovingCamera(const std::vector<Vector2>& trace_on_world, const std::vector<Vector2> camera_positions, const SDL_Color& color);
    void drawText(SDL_Point position, const std::string& text, SDL_Color color, int size = 20);

    void drawRectInWorld(Vector2 top_left, Vector2 bottom_right, SDL_Color color);

    void drawNode(QuadTree::Node *node);
    void drawPlanet(const Planet* planet);
    void drawFps();
protected:
    Painter(SDL_Window *window, SDL_Renderer *renderer, Camera *camera);

    static Painter *instance;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Camera *camera;
    TTF_Font *font;
    Timer *timer;
};

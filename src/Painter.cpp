#include "../include/main.hpp"


Painter* Painter::instance = nullptr;

Painter* Painter::getInstance(SDL_Window *window, SDL_Renderer *renderer, Camera *camera)
{
    if (instance == nullptr)
    {
        instance = new Painter(window, renderer, camera);
    }
    return instance;
}

Painter *Painter::getInstance()
{
    if(instance == nullptr)
        throw std::runtime_error("Painter::getInstance: give the parameters before using this function");
    return instance;
}

Painter::Painter(SDL_Window *window, SDL_Renderer *renderer, Camera *camera)
{
    if(window == nullptr || renderer == nullptr || camera == nullptr)
        throw std::runtime_error("Invalid parameter: window, renderer, and camera must not be null");
    this->window = window;
    this->renderer = renderer;
    this->camera = camera;
    font = nullptr;
    timer = Timer::getInstance();
}

void Painter::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Painter::drawCircle(SDL_Point center, int radius, SDL_Color color)
{
    if(radius <= 0)
        return;

    if(center.x + radius < 0 || center.x - radius > camera->getWindowWidth() || center.y + radius < 0 || center.y - radius > camera->getWindowHeight())
        return;
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int x = radius;
    int y = 0;
    std::vector<SDL_Point> points;
    points.reserve(radius*9);

    while (x >= y) {
        points.push_back({center.x + x, center.y + y});
        points.push_back({center.x + y, center.y + x});
        points.push_back({center.x - y, center.y + x});
        points.push_back({center.x - x, center.y + y});
        points.push_back({center.x - x, center.y - y});
        points.push_back({center.x - y, center.y - x});
        points.push_back({center.x + y, center.y - x});
        points.push_back({center.x + x, center.y - y});

        y ++;
        if (x*x + y*y > radius*radius) {
            x --;
        }
    }
    SDL_RenderDrawPoints(renderer, points.data(), points.size());
}

void Painter::drawFilledCircle(SDL_Point center, int radius, SDL_Color color)
{
    if(radius <= 0)
        return;

    if(center.x + radius < 0 || center.x - radius > camera->getWindowWidth() || center.y + radius < 0 || center.y - radius > camera->getWindowHeight())
        return;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if(radius == 1)
    {
        SDL_RenderDrawPoint(renderer, center.x, center.y);
        return;
    }
    int x = radius;
    int y = 0;
    std::vector<HorizontalLine> lines;
    lines.reserve(radius*3);

    while (x >= y) {
        lines.push_back({center.x + x, center.x - x, center.y + y});
        lines.push_back({center.x + x, center.x - x, center.y - y});
        lines.push_back({center.x + y, center.x - y, center.y + x});
        lines.push_back({center.x + y, center.x - y, center.y - x});

        y ++;
        if (x*x + y*y > radius*radius) {
            x --;
        }
    }

    for(HorizontalLine line : lines) {
        SDL_RenderDrawLine(renderer, line.x1, line.y, line.x2, line.y);
    }
}

void Painter::drawLine(SDL_Point start, SDL_Point end, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void Painter::drawTraceInScreen(const std::vector<SDL_Point>& trace_on_screen, const SDL_Color& color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLines(renderer, trace_on_screen.data(), trace_on_screen.size());
}

void Painter::drawTraceInWorld(const std::vector<Vector2>& trace_on_world, const SDL_Color& color)
{
    std::vector<SDL_Point> trace_on_screen;
    trace_on_screen.reserve(trace_on_world.size());
    for(Vector2 point : trace_on_world)
    {
        trace_on_screen.push_back(camera->worldToScreen(point));
    }
    drawTraceInScreen(trace_on_screen, color);
}

void Painter::drawTraceMovingCamera(const std::vector<Vector2>& trace_on_world, const std::vector<Vector2> camera_positions, const SDL_Color& color)
{
    if(trace_on_world.size() != camera_positions.size())
    {
        throw std::invalid_argument("trace_on_world.size() != camera_positions.size()");
        return;
    }
    std::vector<SDL_Point> trace_on_screen;
    trace_on_screen.reserve(trace_on_world.size());
    Camera camera_copy = *camera;
    for(int i = 0; i < trace_on_world.size(); i++)
    {
        camera_copy.setPosition(camera_positions[i]);
        trace_on_screen.push_back(camera_copy.worldToScreen(trace_on_world[i]));
    }
    drawTraceInScreen(trace_on_screen, color);
}

void Painter::drawText(SDL_Point position, const std::string& text, SDL_Color color, int size)
{
    if(font == nullptr)
    {
        font = TTF_OpenFont("fonts/ChronicaRegular-owr14.ttf", size);
        if(font == nullptr)
        {
            printf("Font not found: %s\n", TTF_GetError());
            exit(1);
        }
    }
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {position.x, position.y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Painter::drawRectInWorld(Vector2 top_left, Vector2 bottom_right, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {static_cast<int>(top_left.x), static_cast<int>(top_left.y), static_cast<int>(bottom_right.x - top_left.x), static_cast<int>(bottom_right.y - top_left.y)};
    SDL_RenderDrawRect(renderer, &rect);
}


void Painter::drawPlanet(const Planet *planet)
{
    SDL_Point position = camera->worldToScreen(planet->getPosition());
    double radius = planet->getRadius()/camera->getZoom();
    SDL_Color color = planet->getColor();

    if(position.x > -1 && position.x < camera->getWindowWidth()+1 && position.y > -1 && position.y < camera->getWindowHeight()+1)
    {
        if(radius < 0.5)
        {
            color.a *= radius*2;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, position.x, position.y);
            return;
        }

        if(radius < 1)
        {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, position.x, position.y);

            color.a *= radius;
            SDL_Point rect[4] = {{position.x - 1, position.y}, {position.x + 1, position.y}, {position.x, position.y + 1}, {position.x, position.y - 1}};
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoints(renderer, rect, 4);
            
            return;
        }
    }
    drawFilledCircle(position, ceil(radius), color);

}

void Painter::drawNode(QuadTree::Node *node)
{
    if(node == nullptr)
        return;

    Vector2 bottom_right = {node->boundaries.top_left.x + node->boundaries.size, node->boundaries.top_left.y + node->boundaries.size};
    drawRectInWorld(node->boundaries.top_left, bottom_right, {0, 0, 255, 255});

    drawNode(node->NE);
    drawNode(node->NW);
    drawNode(node->SE);
    drawNode(node->SW);
}

void Painter::drawFps()
{
    drawText({20, 100}, "FPS: " + std::to_string(timer->getFps()), {255, 255, 255, 255}, 40);
}
#include "../include/main.hpp"


CursorMode::CursorMode(std::vector<Creator *> creators, PlanetManager *planet_manager, SDL_Renderer *renderer)
{
    if(renderer == nullptr)
        throw std::invalid_argument("CursorMode::CursorMode: renderer is nullptr");
    
    if(planet_manager == nullptr)
        throw std::invalid_argument("CursorMode::CursorMode: planet_manager is nullptr");

    this->renderer = renderer;
    this->planet_manager = planet_manager;
    this->creators = creators;
    input_handler = InputHandler::getInstance();
    current_brush = 0;
    for(auto creator : creators)
        textures.push_back(loadTexture(creator->icon_path.c_str(), 0.333));
}

CursorMode::~CursorMode()
{
    for (auto texture : textures)
        SDL_DestroyTexture(texture);
    
    for (auto creator : creators)    
        delete creator;
}


void CursorMode::update()
{
    if (input_handler->isKeyInState(InputHandler::MOUSE_RIGHT, InputHandler::JUST_PRESSED))
    {
        current_brush = (current_brush + 1) % creators.size();
        creators[current_brush]->reset();
    }

    creators[current_brush]->update();
}

void CursorMode::render()
{
    creators[current_brush]->render();
    
    if(!input_handler->isKeyInState(InputHandler::MOUSE_LEFT, InputHandler::PRESSED))
    {
        SDL_Rect rect = {input_handler->getMousePosition().x, input_handler->getMousePosition().y - 50, 50, 50};
        SDL_RenderCopy(renderer, textures[current_brush], NULL, &rect);
    }
}

SDL_Texture *CursorMode::loadTexture(const char *filename, double resize_factor)
{
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)
        throw std::runtime_error("CursorMode::loadTexture: failed to load texture");

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        throw std::runtime_error("CursorMode::loadTexture: failed to create texture");

    SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;
    if (SDL_SetTextureBlendMode(texture, blendMode) != 0)
        throw std::runtime_error("CursorMode::loadTexture: failed to set texture blend mode");

    SDL_FreeSurface(surface);
    return texture;
}
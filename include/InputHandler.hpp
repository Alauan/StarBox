#pragma once
#include "main.hpp"


class InputHandler
{
public:
    InputHandler(const InputHandler& other) = delete;
    void operator=(const InputHandler& other) = delete;

    static InputHandler* getInstance();

    enum States
    {
        PRESSED = 1,
        JUST_PRESSED = 2,
        JUST_RELEASED = 4
    };

    enum Keys
    {
        MOUSE_LEFT = SDL_BUTTON_LEFT, 
        MOUSE_MIDDLE = SDL_BUTTON_MIDDLE,
        MOUSE_RIGHT = SDL_BUTTON_RIGHT
    };

    void getInput();

    bool isKeyInState(Keys key, States state);
    int getMouseScroll();
    SDL_Point getMousePosition();
    SDL_Point getMousePositionDiff();

    bool didQuit();

private:
    InputHandler();
    ~InputHandler();

    static InputHandler* instance;

    void restartVariables();

    std::unordered_map<Keys, Uint16> key_states;
    int mouse_scroll;
    SDL_Point mouse_position;
    SDL_Point mouse_position_diff;
    bool quit;
};

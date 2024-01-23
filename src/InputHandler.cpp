#include "../include/main.hpp"


InputHandler *InputHandler::instance = nullptr;

InputHandler *InputHandler::getInstance()
{
    if(instance == nullptr)
    {
        instance = new InputHandler();
    }
    return instance;
}

InputHandler::InputHandler()
{
    mouse_position = {0, 0};
    mouse_position_diff = {0, 0};
    quit = false;
}

void InputHandler::getInput()
{
    restartVariables();
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                key_states[(Keys)event.button.button] = States::JUST_PRESSED | States::PRESSED;
                break;
            case SDL_MOUSEBUTTONUP:
                key_states[(Keys)event.button.button] = States::JUST_RELEASED;
                key_states[(Keys)event.button.button] &= ~States::PRESSED;
                break;
            case SDL_MOUSEWHEEL:
                mouse_scroll = event.wheel.y;
                break;
            case SDL_MOUSEMOTION:
                mouse_position_diff.x = event.motion.x - mouse_position.x;
                mouse_position_diff.y = event.motion.y - mouse_position.y;
                mouse_position = {event.motion.x, event.motion.y};
                break;
        }
    }
}

void InputHandler::restartVariables()
{
    mouse_scroll = 0;
    mouse_position_diff = {0, 0};
    for(int i = 1; i <= 3; i++)
    {
        key_states[(Keys)i] &= ~States::JUST_PRESSED;
        key_states[(Keys)i] &= ~States::JUST_RELEASED;
    }
}

bool InputHandler::isKeyInState(Keys key, States state)
{
    return key_states[key] & state;
}

int InputHandler::getMouseScroll()
{
    return mouse_scroll;
}

SDL_Point InputHandler::getMousePosition()
{
    return mouse_position;
}

SDL_Point InputHandler::getMousePositionDiff()
{
    return mouse_position_diff;
}

bool InputHandler::didQuit()
{
    return quit;
}

#include "../include/main.hpp"

Core *Core::instance = nullptr;

Core* Core::getInstance()
{
    if(!instance)
    {
        instance = new Core();
    }
    return instance;
}

Core::Core()
{
    window = nullptr;
    renderer = nullptr;
    input_handler = nullptr;
    planet_manager = nullptr;
    painter = nullptr;
    camera = nullptr;
    timer = nullptr;
    running = true;
}


void Core::init()
{
    initSDL();
    running = true;
    input_handler = InputHandler::getInstance();
    timer = Timer::getInstance();
    painter = Painter::getInstance(window, renderer, camera);
    planet_manager = new PlanetManager();

    creators = {new PlanetCreator(camera, planet_manager), 
                new GalaxyCreator(camera, planet_manager), 
                new CosmicDustCreator(camera, planet_manager),
                new Eraser(camera, planet_manager)};

    cursor_mode = new CursorMode(creators, planet_manager, renderer);
}

void Core::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_DisplayMode display_mode;
    if(SDL_GetCurrentDisplayMode(0, &display_mode) != 0)
    {
        std::cout << "SDL could not get display mode! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    camera = new Camera(display_mode.w, display_mode.h);

    window = SDL_CreateWindow("StarBox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, camera->getWindowWidth(), camera->getWindowHeight(), SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
    {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (renderer == NULL)
    {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags))
    {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(1);
    }
}

bool Core::isRunning()
{
    return running;
}

void Core::createFrame()
{
    input_handler->getInput();
    timer->update();
    cursor_mode->update();
    planet_manager->update();
    painter->clearScreen();

    if(input_handler->isKeyInState(InputHandler::MOUSE_MIDDLE, InputHandler::PRESSED))
        camera->move(Vector2(input_handler->getMousePositionDiff()) * camera->getZoom() * -1);

    int scroll = input_handler->getMouseScroll(); 
    if(scroll)
        camera->zoomOnPoint(-scroll*0.1, input_handler->getMousePosition());

    if(input_handler->isKeyInState(InputHandler::MOUSE_MIDDLE, InputHandler::JUST_PRESSED))
        planet_manager->followPlanet(planet_manager->getPlanetAtPosition(camera->screenToWorld(input_handler->getMousePosition())));

    if(planet_manager->isFollowingPlanet())
        camera->setPosition(planet_manager->getFollowedPlanet()->getPosition());


    painter->drawFps();
    planet_manager->render();
    cursor_mode->render();

    SDL_RenderPresent(renderer);

    if(input_handler->didQuit())
        running = false;
}

void Core::close()
{
    delete planet_manager;
    delete painter;
    delete camera;
    delete timer;
    delete cursor_mode;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = nullptr;
    window = nullptr;
    SDL_Quit();
}